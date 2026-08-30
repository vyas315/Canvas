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

#ifndef CANVAS_CORE_LINKREGISTRYADJ_H_
#define CANVAS_CORE_LINKREGISTRYADJ_H_

namespace Canvas
{
  namespace Core
  {
    class LinkRegistryAdj : public LinkRegistry
    {
      private:
        std::unordered_map<unsigned int, std::vector<unsigned int> > registry;
        void addImpl(
            unsigned int A,
            unsigned int B);
        std::vector<std::vector<unsigned int> > getComponentsImpl();
        std::vector<unsigned int> getNeighboursImpl(
            unsigned int root);

      public:
        void add(
            unsigned int A,
            unsigned int B);
        std::vector<std::vector<unsigned int> > getComponents();
        std::vector<unsigned int> getNeighbours(
            unsigned int root);
    };

    void LinkRegistryAdj::addImpl(
        unsigned int A,
        unsigned int B)
    {
      if (registry.find(A) == registry.end())
        registry[A] = std::vector<unsigned int> ();
      if (registry.find(B) == registry.end())
        registry[B] = std::vector<unsigned int> ();

      registry[A].push_back(B);
      registry[B].push_back(A);

      return;
    }

    std::vector<std::vector<unsigned int> >
        LinkRegistryAdj::getComponentsImpl()
    {
      std::vector<std::vector<unsigned int> > components;
      std::set<unsigned int> processed;
      unsigned int index, head, candidateIndex, candidate;

      std::unordered_map<unsigned int, std::vector<unsigned int> >::iterator itr;
      for (itr = registry.begin(); itr != registry.end(); itr++)
      {
        if (processed.find(itr->first) != processed.end()) continue;

        index = 0;
        std::vector<unsigned int> component, candidates;
        component.push_back(itr->first);
        processed.insert(itr->first);
        while (index < component.size())
        {
          head = component[index];
          candidates = registry[head];
          for (candidateIndex = 0; candidateIndex < candidates.size();
              candidateIndex++)
          {
            candidate = candidates[candidateIndex];
            if (processed.find(candidate) != processed.end()) continue;
            component.push_back(candidate);
            processed.insert(candidate);
          }
          index++;
        }
        components.push_back(component);
      }
      return components;
    }

    std::vector<unsigned int> LinkRegistryAdj::getNeighboursImpl(
        unsigned int root)
    {
      if (registry.find(root) == registry.end())
        return std::vector<unsigned int> ();
      return registry[root];
    }

    void LinkRegistryAdj::add(
        unsigned int A,
        unsigned int B)
    { addImpl(A, B); return; }

    std::vector<std::vector<unsigned int> >
        LinkRegistryAdj::getComponents()
    { return getComponentsImpl(); }

    std::vector<unsigned int> LinkRegistryAdj::getNeighbours(
        unsigned int root)
    { return getNeighboursImpl(root); }
    
  }
}

#endif
