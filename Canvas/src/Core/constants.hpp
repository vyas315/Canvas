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

#ifndef CANVAS_CORE_CONSTANTS_H_
#define CANVAS_CORE_CONSTANTS_H_

namespace Canvas
{
  namespace Core
  {
    namespace constants
    {
      enum IntervalQuadrant
      {
        Q1 = 0,
        Q2 = 1,
        Q3 = 2,
        Q4 = 3
      };
      enum FringeType
      {
        LEFT = 0,
        RIGHT = 1,
        TOP = 2,
        BOTTOM = 3
      };
    }
  }
}

#endif
