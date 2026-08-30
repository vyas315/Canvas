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

#ifndef CANVAS_CORE_RELATIONSHUB_H_
#define CANVAS_CORE_RELATIONSHUB_H_

namespace Canvas
{
  namespace Core
  {
    class RelationsHub
    {
      private:
        std::unordered_map<std::string, RelationRegistry*> registries;

      public:
        void add(
            std::string name,
            unsigned int from,
            unsigned int to);
        std::vector<std::tuple<
            std::vector<unsigned int>,
            std::vector<unsigned int> > > getRelated(
            std::string name);
     };

     void RelationsHub::add(
         std::string name,
         unsigned int from,
         unsigned int to)
     {
       if (registries.find(name) == registries.end())
         registries[name] = new RelationRegistryAdj();
       registries[name]->add(from, to);
       return;
     }

     std::vector<std::tuple<
         std::vector<unsigned int>,
         std::vector<unsigned int> > > RelationsHub::getRelated(
         std::string name)
     {
       if (registries.find(name) == registries.end())
         return std::vector<std::tuple<
             std::vector<unsigned int>,
             std::vector<unsigned int> > > ();
       return registries[name]->getRelated();
     }

  }
}

#endif
