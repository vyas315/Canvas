# Copyright 2018-2026 Vedavyas Chigurupati
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import sys

from setuptools import setup
from setuptools.extension import Extension

from Cython.Build import cythonize

HERE = os.path.abspath(os.path.dirname(__file__))

with open(os.path.join(HERE, "README.md"), encoding="utf-8") as readme:
    LONG_DESCRIPTION = readme.read()

# MSVC spells the standard-selection flag differently, and rejects -std=.
if sys.platform == "win32":
    EXTRA_COMPILE_ARGS = ["/std:c++14"]
else:
    EXTRA_COMPILE_ARGS = ["-std=c++11"]

canvas_extension = Extension(
    name="canvas",
    sources=[os.path.join(HERE, "python-binding", "binding.pyx")],
    include_dirs=[HERE],
    extra_compile_args=EXTRA_COMPILE_ARGS,
    language="c++",
)

setup(
    # PyPI distribution name. The import name remains `canvas`, set by the
    # Extension above; `canvas` itself is already taken on PyPI.
    name="canvas-dsl",
    version="1.0.0rc1",
    description=(
        "A domain-specific language for extracting structured data from "
        "documents by spatial reasoning."
    ),
    long_description=LONG_DESCRIPTION,
    long_description_content_type="text/markdown",
    author="Vedavyas Chigurupati",
    url="https://github.com/vyas315/Canvas",
    project_urls={
        "Source": "https://github.com/vyas315/Canvas",
        "Issues": "https://github.com/vyas315/Canvas/issues",
    },
    license="Apache-2.0",
    ext_modules=cythonize([canvas_extension]),
    python_requires=">=3.8",
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: Apache Software License",
        "Programming Language :: C++",
        "Programming Language :: Cython",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: Python :: 3.13",
        "Operating System :: OS Independent",
        "Topic :: Scientific/Engineering :: Image Recognition",
        "Topic :: Text Processing :: Markup",
    ],
    keywords="dsl document-parsing pdf table-extraction spatial gestalt",
)
