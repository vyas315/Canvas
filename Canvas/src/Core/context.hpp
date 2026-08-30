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

#ifndef CANVAS_CORE_CONTEXT_H_
#define CANVAS_CORE_CONTEXT_H_

namespace Canvas
{
  namespace Core
  {
    class Context
    {
      private:
        LinksHub* linksHub;
        RegionsHub* regionsHub;
        RelationsHub* relationsHub;

      public:
        LinksHub* getLinksHub();
        RegionsHub* getRegionsHub();
        RelationsHub* getRelationsHub();
        Context();
    };

    LinksHub* Context::getLinksHub()
    { return linksHub; }

    RegionsHub* Context::getRegionsHub()
    { return regionsHub; }

    RelationsHub* Context::getRelationsHub()
    { return relationsHub; }

    Context::Context()
    {
      linksHub = new LinksHub();
      regionsHub = new RegionsHub();
      relationsHub = new RelationsHub();
    }

  }
}

#endif
