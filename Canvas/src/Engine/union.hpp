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

#ifndef CANVAS_ENGINE_UNION_H_
#define CANVAS_ENGINE_UNION_H_

namespace Canvas
{
  namespace Engine
  {
    void unionProc(
        Core::Context* context,
        std::string A,
        std::string B,
        std::string target)
    {
      Core::RegionsHub* regionsHub = context->getRegionsHub();
      Core::Region searchRegion = Core::Region(-Constants::FLOAT_MAX,
          -Constants::FLOAT_MAX, Constants::FLOAT_MAX, Constants::FLOAT_MAX);

      std::vector<std::string> variables, values;
      variables.push_back("source");
      values.push_back(A);

      Handler::Query* query = new Handler::QueryIntersect();
      query->setContext(context, variables, values);
      std::vector<Core::Region> regions = query->operator()(searchRegion);
      std::set<unsigned int> included;
      for (unsigned int index = 0; index < regions.size(); index++)
      {
        regionsHub->add(target, regions[index]);
        included.insert(regions[index].getId());
      }

      values[0] = B;
      query->setContext(context, variables, values);
      regions = query->operator()(searchRegion);
      for (unsigned int index = 0; index < regions.size(); index++)
      {
        if (included.find(regions[index].getId()) != included.end()) continue;
        regionsHub->add(target, regions[index]);
        included.insert(regions[index].getId());
      }

      return;
    }

    void unionImpl(
        Core::Context* context,
        std::string command)
    {
      preProcess(command);
      std::regex cmdRegex("union\\{(.*),(.*)\\}into\\{(.*)\\}");
      std::smatch result;

      std::regex_search(command, result, cmdRegex);
      unionProc(context, result[1], result[2], result[3]);
      return;
    }

    void union_(
        Core::Context* context,
        std::string command)
    {
      unionImpl(context, command);
      return;
    }

  }
}

#endif
