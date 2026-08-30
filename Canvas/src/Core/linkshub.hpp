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

#ifndef CANVAS_CORE_LINKSHUB_H_
#define CANVAS_CORE_LINKSHUB_H_

namespace Canvas
{
  namespace Core
  {
    class LinksHub
    {
      private:
        std::unordered_map<std::string, LinkRegistry*> registries;

      public:
        void add(
            std::string name,
            unsigned int A,
            unsigned int B);
        std::vector<std::vector<unsigned int> > getComponents(
            std::string name);
        std::vector<unsigned int> getNeighbours(
            std::string name,
            unsigned int root);
     };

     void LinksHub::add(
         std::string name,
         unsigned int A,
         unsigned int B)
     {
       if (registries.find(name) == registries.end())
         registries[name] = new LinkRegistryAdj();
       registries[name]->add(A, B);
       return;
     }

     std::vector<std::vector<unsigned int> > LinksHub::getComponents(
         std::string name)
     {
       if (registries.find(name) == registries.end())
         return std::vector<std::vector<unsigned int> > ();
       return registries[name]->getComponents();
     }

     std::vector<unsigned int> LinksHub::getNeighbours(
         std::string name,
         unsigned int root)
     {
       if (registries.find(name) == registries.end())
         return std::vector<unsigned int> ();
       return registries[name]->getNeighbours(root);
     }

  }
}

#endif
