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

#ifndef CANVAS_ENGINE_UTILS_H_
#define CANVAS_ENGINE_UTILS_H_

namespace Canvas
{
  namespace Engine
  {
    std::tuple<std::vector<std::string>, std::vector<std::string> >
        getParameters(
        std::string command)
    {
      std::vector<std::string> parameters;
      std::vector<std::string> values;

      std::stringstream outer(command);
      while (command.length() > 3 && outer.good())
      {
        std::string substr;
        std::getline(outer, substr, ',');
        std::stringstream inner(substr);

        unsigned int index = 0;
        while (inner.good())
        {
          std::string innerstr;
          std::getline(inner, innerstr, '=');
          if (index == 0) parameters.push_back(innerstr);
          else if (index == 1) values.push_back(innerstr);
          index++;
        }
      }

      return std::tuple<std::vector<std::string>, std::vector<std::string> >(
          parameters, values);
    }
  }
}

#endif
