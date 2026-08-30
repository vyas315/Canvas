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

#ifndef CANVAS_CORE_REGIONSHUB_H_
#define CANVAS_CORE_REGIONSHUB_H_

namespace Canvas
{
  namespace Core
  {
    class RegionsHub
    {
      private:
        std::unordered_map<std::string, RegionRegistry*> registries;
        std::string activeRegistry;

      public:
        void setActiveRegistry(
            std::string name);
        void add(
            std::string name,
            Region region);
        void add(Region region);
        std::vector<Region> intersectQuery(
            std::string name,
            Region queryRegion);
        std::vector<Region> intersectQuery(
            Region queryRegion);
        std::vector<Region> fringeQuery(
            std::string name,
            Region queryRegion,
            constants::FringeType fringeType);
        std::vector<Region> fringeQuery(
            Region queryRegion,
            constants::FringeType fringeType);
        std::vector<Region> getRegionsByIds(
            std::string name,
            std::vector<unsigned int> ids);
        std::vector<Region> getRegionsByIds(
            std::vector<unsigned int> ids);
    };

    void RegionsHub::setActiveRegistry(
        std::string name)
    { activeRegistry = name; }

    void RegionsHub::add(
        std::string name,
        Region region)
    {
      if (registries.find(name) == registries.end())
        registries[name] = new RegionRegistryInterval();
      registries[name]->add(region);
      return;
    }

    void RegionsHub::add(
        Region region)
    { add(activeRegistry, region); return; }

    std::vector<Region> RegionsHub::intersectQuery(
        std::string name,
        Region queryRegion)
    {
      if (registries.find(name) == registries.end())
        return std::vector<Region> ();
      return registries[name]->intersectQuery(queryRegion);
    }

    std::vector<Region> RegionsHub::intersectQuery(
        Region queryRegion)
    { return intersectQuery(activeRegistry, queryRegion); }

    std::vector<Region> RegionsHub::fringeQuery(
        std::string name,
        Region queryRegion,
        constants::FringeType fringeType)
    {
      if (registries.find(name) == registries.end())
        return std::vector<Region> ();
      return registries[name]->fringeQuery(queryRegion, fringeType);
    }

    std::vector<Region> RegionsHub::fringeQuery(
        Region queryRegion,
        constants::FringeType fringeType)
    { return fringeQuery(activeRegistry, queryRegion, fringeType); }

    std::vector<Region> RegionsHub::getRegionsByIds(
        std::string name,
        std::vector<unsigned int> ids)
    {
      if (registries.find(name) == registries.end())
        return std::vector<Region> ();
      return registries[name]->getRegionsByIds(ids);
    }

    std::vector<Region> RegionsHub::getRegionsByIds(
        std::vector<unsigned int> ids)
    { return getRegionsByIds(activeRegistry, ids); }

  }
}

#endif
