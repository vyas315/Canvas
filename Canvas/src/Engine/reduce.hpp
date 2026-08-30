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

#ifndef CANVAS_ENGINE_REDUCE_H_
#define CANVAS_ENGINE_REDUCE_H_

namespace Canvas
{
  namespace Engine
  {
    void reduceProc(
        Core::Context* context,
        std::string source,
        std::string target,
        std::string link,
        Handler::Reduce* handler)
    {
      Core::LinksHub* linksHub = context->getLinksHub();
      Core::RegionsHub* regionsHub = context->getRegionsHub();

      std::vector<std::vector<unsigned int> > components =
          linksHub->getComponents(link);

      std::vector<Core::Region> component;
      for (unsigned int componentIndex = 0; componentIndex < components.size();
          componentIndex++)
      {
        component = regionsHub->getRegionsByIds(source,
            components[componentIndex]);
        regionsHub->add(target, handler->operator()(component));
      }

      return;
    }

    void reduceImpl(
        Core::Context* context,
        std::string command)
    {
      preProcess(command);
      std::regex cmdRegex("reduce\\{(.*)\\}into\\{(.*)\\}using(.*)\\((.*)\\)observing\\{(.*)\\}");
      std::smatch result;

      std::regex_search(command, result, cmdRegex);
      Handler::Reduce* handler = getReduceHandler(result[3]);

      std::tuple<std::vector<std::string>, std::vector<std::string> >
          parameters = getParameters(result[4]);
      for (unsigned int index = 0; index < (std::get<0> (parameters)).size();
          index++)
        handler->setParameter((std::get<0> (parameters))[index],
            (std::get<1> (parameters))[index]);

      reduceProc(context, result[1], result[2], result[5], handler);
      return;
    }

    void reduce(
        Core::Context* context,
        std::string command)
    {
      reduceImpl(context, command);
      return;
    }

  }
}

#endif
