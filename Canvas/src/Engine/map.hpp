/*
 * Copyright 2018-2026 Vedavyas Chigurupati
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CANVAS_ENGINE_MAP_H_
#define CANVAS_ENGINE_MAP_H_

namespace Canvas
{
  namespace Engine
  {
    Handler::Query* getQueryHandler(
        std::string name)
    {
      if (name == "AdjLeft") return new Handler::QueryAdjLeft();
      else if (name == "AdjRight") return new Handler::QueryAdjRight();
      else if (name == "AdjBottom") return new Handler::QueryAdjBottom();
      else if (name == "AdjTop") return new Handler::QueryAdjTop();
      else if (name == "Intersect") return new Handler::QueryIntersect();
    }

    Handler::Reduce* getReduceHandler(
        std::string name)
    {
      if (name == "Merge") return new Handler::ReduceMerge();
      else if (name == "LeftRightConcat")
          return new Handler::ReduceLeftRightConcat();
      else if (name == "TopBottomConcat")
          return new Handler::ReduceTopBottomConcat();
    }

    Handler::Coalesce* getCoalesceHandler(
        std::string name)
    {
      if (name == "AreaValue") return new Handler::CoalesceAreaValue();
      if (name == "Area") return new Handler::CoalesceArea();
    }

    Handler::Split* getSplitHandler(
        std::string name)
    {
      if (name == "Left") return new Handler::SplitLeft();
      if (name == "Top") return new Handler::SplitTop();
      else if (name == "VerticalProjection") return new Handler::SplitVerticalProjection();
    }

  }
}

#endif
