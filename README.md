# Canvas

**A domain-specific language for extracting structured data from documents by spatial reasoning.**

Canvas turns a bag of positioned rectangles — the glyphs and rulings on a page — into structured records. It does this with seven verbs and a small vocabulary of perceptual primitives borrowed from Gestalt psychology.

The engine has **no built-in concept of a table, a cell, a row, a header, or a document**. It knows regions, grouping, and association. Everything else emerges from the sequence of statements you write.

```python
from canvas import Stub, Region

stub = Stub()
for ch in page.characters:              # each character's bounding box + value
    stub.add("Letters", ch)

stub.execute("link   {Letters} with {LetterLinks} using AdjLeft(maxDistance = 3, trimVertical = 0.4)")
stub.execute("reduce {Letters} into {Words} using LeftRightConcat() observing {LetterLinks}")

for word in stub.ordered_retrieve("Words", "minY"):
    print(word.value, word.x1, word.y1, word.x2, word.y2)
```

Two statements turn a page of loose glyphs into words — by proximity, with no notion of what a word is.

---

## Table of contents

- [Why a DSL](#why-a-dsl)
- [Installation](#installation)
- [Core concepts](#core-concepts)
- [Language reference](#language-reference)
- [Handler reference](#handler-reference)
- [Python API](#python-api)
- [A complete example](#a-complete-example)
- [How it works](#how-it-works)
- [Status and limitations](#status-and-limitations)
- [Roadmap](#roadmap)
- [Development](#development)
- [License](#license)

---

## Why a DSL

Rendering is a one-way function. A layout engine consumes a semantic structure — a `<table>`, a `tabular` environment, a report generator's row loop — and emits geometry. The structure is discarded. PDF in particular is a page-description format with no semantic layer to recover.

So extraction is an ill-posed inverse problem, and like any ill-posed inverse problem it needs a prior to select among the possible answers. Canvas is a language for writing that prior.

Two approaches usually get tried first, and both have a characteristic failure:

- **One general algorithm steered by a config file** becomes unmaintainable, because in a general-purpose language the code that *computes the inputs to a decision*, the code that *makes the decision*, and the code that *applies the result* all look identical and nothing stops them interleaving.
- **A model per document format** works but doesn't compose — nothing can be partially reused, and the maintenance surface grows without bound.

Canvas makes the **sequence** the unit of reuse, and separates those three concerns into three syntactic slots:

```
reduce {Letters} into {Words} using LeftRightConcat() observing {LetterLinks}
                               └── the decision      └── the evidence
       └── the effect
```

`using` names the perceptual criterion. `observing` names the previously-computed evidence it runs over. `into` names where the result goes. You cannot smuggle a computation into the `observing` slot — it only accepts the name of an edge set some earlier statement built.

The primitives come from perception rather than from document semantics, because a rendered page really was produced by an engine placing things according to proximity, alignment, and separation. A vocabulary lifted from human vision sits close to the actual generative factors of the artifact:

| Gestalt principle | Canvas operation |
|---|---|
| Proximity | `AdjLeft`, `AdjRight`, `AdjTop`, `AdjBottom` with `maxDistance` |
| Similarity / alignment | `Intersect(minOverlap)`, and the `trim` parameters that define an alignment band |
| Continuity | `Merge`, joining broken line fragments into continuous rulings |
| Closure | `split`, synthesising cells from the rulings that bound them |
| Figure / ground | `VerticalProjection`, finding columns as valleys in a whitespace profile |

---

## Installation

Canvas is a header-only C++ library with Cython bindings. You need a C++ compiler and Cython.

```bash
git clone https://github.com/vyas315/Canvas.git
cd Canvas
pip install .
```

Requirements:

- Python 3.6+
- Cython
- A C++ compiler supporting C++11

---

## Core concepts

### Region

The only value type. A bounding box, a string, and a process-global identifier.

```python
Region(x1, y1, x2, y2, value="")
```

A glyph is a Region. A ruled line is a Region. A word, a cell, a row, the whole page — all Regions. There is no type hierarchy, which is why the same statement that groups characters into words also groups line fragments into rulings.

**Coordinates are Y-up** — `y2 > y1` means "higher on the page" — matching the PDF coordinate system.

Identifiers are unique across every registry in the process, which is what makes `union` and `intersect` meaningful as set operations on *identity* rather than geometry.

### Registries

Named sets of Regions, created on first write. Registry names are ordinary strings (`Letters`, `Words`, `VerticalLines`, `Cells`), and each is backed by a spatial index supporting two query shapes: all regions overlapping a box, and the single extreme region in a given direction.

### Links and relations

Canvas records two kinds of relationship, and they answer different questions:

| | Scope | Direction | Read as | Answers |
|---|---|---|---|---|
| **Link** | within one registry | undirected | connected components | *which of these are the same thing?* |
| **Relation** | between two registries | directed, one-to-many | `(from, [to…])` groups | *which of those belong to this?* |

Characters that form a word are **linked**. The words inside a cell are **related** to it. That single distinction is why seven verbs are sufficient.

---

## Language reference

A Canvas program is a sequence of statements, each executed with `stub.execute(...)`. Statements are side-effecting: they read named registries and append to another.

```
link      {S}    with {L} using Query(...)                       build a link graph
relate    {A,B}  with {R} using Query(...)                       build a relation
reduce    {S}    into {T} using Reducer(...)    observing {L}    fold each group into one region
coalesce  {A,B}  into {T} using Coalescer(...)  observing {R}    fold each association into one region
split     {A,B}  into {T} using Splitter(...)   observing {R}    cut one region into many
union     {A,B}  into {T}                                        set union by region identity
intersect {A,B}  into {T}                                        set intersection by region identity
```

The verb set is not arbitrary — it is the closure of the operations the data model admits:

| Edge type | Build it | Consume → one region | Consume → many regions |
|---|---|---|---|
| **Link** (homogeneous) | `link` | `reduce` | — |
| **Relation** (heterogeneous) | `relate` | `coalesce` | `split` |

plus `union` and `intersect` as the set-operation pair. The empty cell is empty by necessity: a split needs a *cutter* drawn from a different set than the thing being cut, which makes splitting inherently heterogeneous.

**Whitespace is insignificant** — it is stripped before parsing. This also means **parameter values may not contain spaces**.

---

## Handler reference

Handlers plug into the `using` slot. Adding one is a class plus a line in a dispatch map; the grammar never changes.

### Queries — used by `link` and `relate`

| Handler | Parameters | Behaviour |
|---|---|---|
| `Intersect` | `minOverlap` | Matches candidates whose overlap with the query region, **as a fraction of the candidate's own area**, exceeds the threshold. |
| `AdjLeft` | `maxDistance`, `trimVertical` | The single nearest region to the left, within `maxDistance`. |
| `AdjRight` | `maxDistance`, `trimVertical` | The single nearest region to the right. |
| `AdjTop` | `maxDistance`, `trimHorizontal` | The single nearest region above. |
| `AdjBottom` | `maxDistance`, `trimHorizontal` | The single nearest region below. |

`trimVertical` narrows the search band vertically by a fraction of the query region's height, applied to both edges — so `trimVertical = 0.4` searches only the middle 20%. This is how "on the same text line" is expressed in an engine with no concept of a line. `trimHorizontal` is the analogue for the vertical queries.

Every `Adj*` query returns **at most one** region.

### Reducers — used by `reduce`

Fold a connected component into one region whose box is the union of its members'.

| Handler | Parameters | Behaviour |
|---|---|---|
| `Merge` | `delimiter` | Concatenates values in unspecified order. Use only when values are empty (e.g. ruled lines). |
| `LeftRightConcat` | `delimiter` | Sorts by left edge, then concatenates. Characters → words. |
| `TopBottomConcat` | `delimiter` | Sorts top to bottom, then concatenates. |

### Coalescers — used by `coalesce`

Fold a `(from, [to…])` relation group into one region.

| Handler | Parameters | Behaviour |
|---|---|---|
| `Area` | — | Bounding box of `from` **and** `to` together; empty value. |
| `AreaValue` | `delimiter`, `order` | Geometry from **`from`**, value from **`to`**, concatenated in reading order. Preserves the source region's identifier. |

`AreaValue`'s reading order treats two regions as being on the same line when their top edges differ by less than 20% of the shorter one's height; same line sorts left-to-right, otherwise top-to-bottom. Pass `order = y` to sort purely vertically.

### Splitters — used by `split`

Cut a `from` region into many, using the `to` regions as cutters.

| Handler | Parameters | Behaviour |
|---|---|---|
| `Left` | — | Cuts at the right edge of each cutter, left to right. |
| `Top` | — | Cuts at the top edge of each cutter, top to bottom. |
| `VerticalProjection` | `anchorCount` | Infers column boundaries from whitespace, for documents with no ruling lines. |

`VerticalProjection` builds an occupancy profile using only the first `anchorCount` rows to locate candidate gutters, then places each boundary at the minimum of the full profile within its gutter band. A sparse, reliable signal locates the feature; a dense signal places it precisely.

---

## Python API

```python
from canvas import Stub, Region
```

### `Region(x1, y1, x2, y2, value="")`

| Member | Type | Description |
|---|---|---|
| `.x1` `.y1` `.x2` `.y2` | `float` | Bounding box, Y-up |
| `.value` | `str` | Associated text |
| `.id` | `int` | Identifier |

Getter methods (`get_x1()`, `get_value()`, …) are available alongside the properties.

### `Stub()`

| Method | Description |
|---|---|
| `add(registry, region)` | Insert a Region into a named registry, creating it if needed. |
| `execute(command)` | Run one Canvas statement. |
| `retrieve(registry)` | Return every Region in a registry, as a list. |
| `ordered_retrieve(registry, order_by)` | As above, sorted. `order_by` is one of `minX`, `maxX`, `minY`, `maxY`. |

---

## A complete example

A table recogniser for a ruled document, in eighteen statements. `Letters` holds glyph boxes; `VerticalBlocks` and `HorizontalBlocks` hold ruling fragments.

```python
directives = [
    # glyphs → words, by overlap and proximity
    "link   {Letters} with {LetterLinks} using Intersect(minOverlap = 0.9)",
    "link   {Letters} with {LetterLinks} using AdjLeft(maxDistance = 3, trimVertical = 0.4)",
    "reduce {Letters} into {Words} using LeftRightConcat() observing {LetterLinks}",

    # ruling fragments → continuous rules, by continuity
    "link   {VerticalBlocks} with {VBlockLinks} using Intersect(minOverlap = 0.9)",
    "link   {VerticalBlocks} with {VBlockLinks} using AdjBottom(maxDistance = 10)",
    "reduce {VerticalBlocks} into {VerticalLines} using Merge() observing {VBlockLinks}",

    "link   {HorizontalBlocks} with {HBlockLinks} using Intersect(minOverlap = 0.9)",
    "link   {HorizontalBlocks} with {HBlockLinks} using AdjLeft(maxDistance = 10)",
    "reduce {HorizontalBlocks} into {HorizontalLines} using Merge() observing {HBlockLinks}",

    # consecutive horizontal rules bound a row, by closure
    "relate   {HorizontalLines, HorizontalLines} with {LineAdj} using AdjBottom()",
    "coalesce {HorizontalLines, HorizontalLines} into {TableRows} using Area() observing {LineAdj}",

    # each row is cut by the vertical rules crossing it
    "relate {TableRows, VerticalLines} with {ColumnIntersects} using Intersect()",
    "split  {TableRows, VerticalLines} into {Cells} using Left() observing {ColumnIntersects}",

    # words fall into cells
    "relate   {Cells, Words} with {CellContents} using Intersect(minOverlap = 0.5)",
    "coalesce {Cells, Words} into {FilledCells} using AreaValue(delimiter = ^) observing {CellContents}",

    # empty cells survive: a left outer join on region identity
    "union {FilledCells, Cells} into {TableCells}",

    # cells assemble into rows
    "relate   {TableRows, TableCells} with {RowContents} using Intersect(minOverlap = 0.5)",
    "coalesce {TableRows, TableCells} into {Rows} using AreaValue(delimiter = |) observing {RowContents}",
]

for directive in directives:
    stub.execute(directive)

for row in stub.ordered_retrieve("Rows", "maxY"):
    columns = row.value.replace("^", " ").split("|")
    print(columns)
```

Two details worth understanding, because they are what separate this from a demo:

**`union {FilledCells, Cells}` is a left outer join.** A cell containing no words produces no `FilledCell`; drop it and a six-column row silently becomes five, shifting every value one position left. `AreaValue` preserves the source region's identifier, so a `FilledCell` *is* its `Cell` by identity, and `union` deduplicates on identity with the `A` operand winning. Filled cells win, empty cells survive with their geometry intact, and the column count stays stable.

**The result is a serialised table inside a region's value.** Words within a cell join with `^`, cells within a row join with `|`, so the final output of the spatial computation is an ordinary string that ordinary code can consume with `split`.

Nothing in the program or the engine mentions tables, banking, dates, or amounts.

---

## How it works

Regions are stored in a 4-way spatial tree: each node holds a region, children are bucketed by quadrant relative to the node's lower-left corner, and every node caches the bounding box of its subtree so queries can prune whole branches.

Two query shapes are built on it, and the second is what makes the language possible:

- **Intersect query** — every region overlapping a box.
- **Fringe query** — *exactly one* region: the extreme one in a given direction, within a box.

The fringe query is why `AdjLeft` is four lines with no loop and no sorting: build a window extending leftward, narrowed vertically, and ask for the region whose right edge is furthest right. Pushing "extremum in a direction" down into the index is what let the language stay declarative with zero control flow.

Execution is a small interpreter: whitespace is stripped, a regular expression identifies the verb, operands are extracted, the named handler is instantiated from a dispatch map, and the operation runs against the context.

---

## Status and limitations

Canvas has been used in production for parsing bank statements across several document formats. It is stable for that use, and it has rough edges you should know about before adopting it.

**Errors are silent.** This is the most important thing to be aware of. Today:

- A misspelled registry name creates a new empty registry rather than raising.
- An unknown handler name is not diagnosed.
- A misspelled parameter is ignored, leaving the default in place.
- A statement that fails to parse is a no-op.

In practice this means a typo produces an empty result rather than an error message. Fixing this is the top item on the roadmap.

**No control flow.** There are no conditionals, loops, or variables. Data-dependent decisions are made by the host program between `execute` calls, which works well in practice but means a Canvas program is a straight line.

**No scoping.** Registry names are global and registries only grow. Running two independent programs against one `Stub` requires prefixing registry names by hand.

**The spatial index is unbalanced.** Insertion order determines shape, and glyphs arriving in reading order are close to sorted, so the tree degrades toward a list. This is most of the available performance headroom.

**Positional parameters are not supported.** Write `AdjLeft(maxDistance = 500)`, not `AdjLeft(500)`.

**Region attributes are limited** to a bounding box and a string — no font, weight, size, or colour, so similarity cues based on styling are not currently expressible.

---

## Roadmap

1. **Diagnostics.** Raise on unknown registries, handlers, parameters, and unparseable statements, naming the offending token. This is the highest-value change in the project.
2. **Scoped registries**, replacing prefix-by-convention with real namespacing.
3. **A balanced or bulk-loaded spatial index.**
4. **Region attributes**, enabling similarity handlers based on styling.
5. **Control flow** — enough that host-language escape hatches become unnecessary.
6. **A hand-written parser** replacing the regular expressions, with positional parameter support.

---

## Development

```
Canvas/
├── Constants           # umbrella headers
├── Core                #   include "Canvas/Core", etc.
├── Engine
├── Handler
├── Main
└── src/
    ├── Core/           # Region, registries, spatial index, hubs, context
    ├── Engine/         # statement parsing and dispatch, one file per verb
    ├── Handler/        # queries, reducers, coalescers, splitters
    └── Main/           # the Stub facade
python-binding/         # Cython .pyx / .pxd
```

Adding a handler is two steps: write the class in `src/Handler/`, then register its name in the dispatch function in `src/Engine/map.hpp`. The grammar does not change.

The C++ is header-only, so there is nothing to link — `#include "Canvas/Main"` and add the repository root to your include path.

---

## Further reading

Two write-ups covering the design in more depth:

- [**Draw the Answer Back on the Page**](https://www.vedavyas.com/blog) — the motivation, the Gestalt basis, and why an inspectable spatial DSL is a good compilation target for language models.
- [**Seven Verbs and a Spatial Index**](https://www.vedavyas.com/blog) — the internals: data model, spatial index, grammar, handler semantics, and an annotated worked example.

---

## License

Licensed under the [Apache License, Version 2.0](LICENSE).

## Contributing

Issues and pull requests are welcome. If you are adding a handler, please include a short example program demonstrating it, and note which Gestalt principle it expresses.
