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

#ifndef CANVAS_ENGINE_RELATE_H_
#define CANVAS_ENGINE_RELATE_H_

namespace Canvas
{
  namespace Engine
  {
    void relateProc(
        Core::Context* context,
        std::string name,
        std::string from,
        std::string to,
        Handler::Query* query)
    {
      std::vector<std::string> variables, values;
      variables.push_back("source");
      values.push_back(from);

      Core::RelationsHub* relationsHub = context->getRelationsHub();
      Handler::Query* sourceQuery = new Handler::QueryIntersect();

      sourceQuery->setContext(context, variables, values);
      std::vector<Core::Region> sourceRegions = sourceQuery->operator()(
        Core::Region(-Constants::FLOAT_MAX, -Constants::FLOAT_MAX,
        Constants::FLOAT_MAX, Constants::FLOAT_MAX));

      values[0] = to;
      query->setContext(context, variables, values);

      unsigned int sourceIndex, targetIndex;
      std::vector<Core::Region> targetRegions;
      for (sourceIndex = 0; sourceIndex < sourceRegions.size(); sourceIndex++)
      {
        targetRegions = query->operator()(sourceRegions[sourceIndex]);
        for (targetIndex = 0; targetIndex < targetRegions.size(); targetIndex++)
          relationsHub->add(name, sourceRegions[sourceIndex].getId(),
              targetRegions[targetIndex].getId());
      }

      return;
    }

    void relateImpl(
        Core::Context* context,
        std::string command)
    {
      preProcess(command);
      std::regex cmdRegex("relate\\{(.*),(.*)\\}with\\{(.*)\\}using(.*)\\((.*)\\)");
      std::smatch result;

      std::regex_search(command, result, cmdRegex);
      Handler::Query* query = getQueryHandler(result[4]);

      std::tuple<std::vector<std::string>, std::vector<std::string> >
          parameters = getParameters(result[5]);
      for (unsigned int index = 0; index < (std::get<0> (parameters)).size();
          index++)
        query->setParameter((std::get<0> (parameters))[index],
            (std::get<1> (parameters))[index]);

      relateProc(context, result[3], result[1], result[2], query);
      return;
    }

    void relate(
        Core::Context* context,
        std::string command)
    {
      relateImpl(context, command);
      return;
    }

  }
}

#endif
