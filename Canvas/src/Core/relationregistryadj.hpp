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

#ifndef CANVAS_CORE_RELATIONREGISTRYADJ_H_
#define CANVAS_CORE_RELATIONREGISTRYADJ_H_

namespace Canvas
{
  namespace Core
  {
    class RelationRegistryAdj : public RelationRegistry
    {
      private:
        std::unordered_map<unsigned int, std::vector<unsigned int> > registry;
        void addImpl(
            unsigned int from,
            unsigned int to);
        std::vector<std::tuple<
            std::vector<unsigned int>,
            std::vector<unsigned int> > > getRelatedImpl();

      public:
        void add(
            unsigned int from,
            unsigned int to);
        std::vector<std::tuple<
            std::vector<unsigned int>,
            std::vector<unsigned int> > > getRelated();
    };

    void RelationRegistryAdj::addImpl(
        unsigned int from,
        unsigned int to)
    {
      if (registry.find(from) == registry.end())
        registry[from] = std::vector<unsigned int> ();
      registry[from].push_back(to);
      return;
    }

    std::vector<std::tuple<
        std::vector<unsigned int>,
        std::vector<unsigned int> > >
        RelationRegistryAdj::getRelatedImpl()
    {
      std::vector<std::tuple<
          std::vector<unsigned int>,
          std::vector<unsigned int> > > result;

      std::unordered_map<unsigned int, std::vector<unsigned int> >::iterator itr;
      for (itr = registry.begin(); itr != registry.end(); itr++)
      {
        std::vector<unsigned int> from;
        from.push_back(itr->first);
        std::vector<unsigned int> to = registry[itr->first];

        result.push_back(std::tuple<std::vector<unsigned int>,
            std::vector<unsigned int> > (from, to));
      }
      return result;
    }

    void RelationRegistryAdj::add(
        unsigned int from,
        unsigned int to)
    { addImpl(from, to); return; }

    std::vector<std::tuple<
        std::vector<unsigned int>,
        std::vector<unsigned int> > >
        RelationRegistryAdj::getRelated()
    { return getRelatedImpl(); }

  }
}

#endif
