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

#ifndef CANVAS_ENGINE_COALESCE_H_
#define CANVAS_ENGINE_COALESCE_H_

namespace Canvas
{
  namespace Engine
  {
    void coalesceProc(
        Core::Context* context,
        std::string from,
        std::string to,
        std::string target,
        std::string relation,
        Handler::Coalesce* handler)
    {
      Core::RelationsHub* relationsHub = context->getRelationsHub();
      Core::RegionsHub* regionsHub = context->getRegionsHub();

      std::vector<std::tuple<
          std::vector<unsigned int>,
          std::vector<unsigned int> > > related =
          relationsHub->getRelated(relation);

      std::vector<Core::Region> fromRegions, toRegions;
      for (unsigned int index = 0; index < related.size(); index++)
      {
        fromRegions = regionsHub->getRegionsByIds(from, std::get<0> (related[index]));
        toRegions = regionsHub->getRegionsByIds(to, std::get<1> (related[index]));
        regionsHub->add(target, handler->operator()(fromRegions, toRegions));
      }
      return;
    }

    void coalesceImpl(
        Core::Context* context,
        std::string command)
    {
      preProcess(command);
      std::regex cmdRegex("coalesce\\{(.*),(.*)\\}into\\{(.*)\\}using(.*)\\((.*)\\)observing\\{(.*)\\}");
      std::smatch result;

      std::regex_search(command, result, cmdRegex);
      Handler::Coalesce* handler = getCoalesceHandler(result[4]);

      std::tuple<std::vector<std::string>, std::vector<std::string> >
          parameters = getParameters(result[5]);
      for (unsigned int index = 0; index < (std::get<0> (parameters)).size();
          index++)
        handler->setParameter((std::get<0> (parameters))[index],
            (std::get<1> (parameters))[index]);
      coalesceProc(context, result[1], result[2], result[3], result[6], handler);
      return;
    }

    void coalesce(
        Core::Context* context,
        std::string command)
    {
      coalesceImpl(context, command);
      return;
    }

  }
}

#endif
