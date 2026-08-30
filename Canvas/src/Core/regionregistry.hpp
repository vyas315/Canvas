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

#ifndef CANVAS_CORE_REGIONREGISTRY_H_
#define CANVAS_CORE_REGIONREGISTRY_H_

namespace Canvas
{
  namespace Core
  {
    class RegionRegistry
    {
      public:
        virtual void add(Region region) = 0;
        virtual std::vector<Region> intersectQuery(Region region) = 0;
        virtual std::vector<Region> fringeQuery(
            Region region,
            constants::FringeType fringeType) = 0;
        virtual std::vector<Region> getRegionsByIds(
            std::vector<unsigned int> ids) = 0;
    };
  }
}

#endif
