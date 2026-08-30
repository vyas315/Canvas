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

#ifndef CANVAS_ENGINE_MAIN_H_
#define CANVAS_ENGINE_MAIN_H_

namespace Canvas
{
  namespace Engine
  {
    void execute(
        Core::Context* context,
        std::string command)
    {
      preProcess(command);
      std::regex cmdRegex("(link|relate|reduce|coalesce|union|intersect|split)\\{(.*)");
      std::smatch result;

      std::regex_search(command, result, cmdRegex);
      if (result[1] == "link") link(context, command);
      else if (result[1] == "relate") relate(context, command);
      else if (result[1] == "reduce") reduce(context, command);
      else if (result[1] == "coalesce") coalesce(context, command);
      else if (result[1] == "union") union_(context, command);
      else if (result[1] == "intersect") intersect(context, command);
      else if (result[1] == "split") split(context, command);

      return;
    }

  }
}

#endif
