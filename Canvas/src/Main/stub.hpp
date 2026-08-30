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

#ifndef CANVAS_MAIN_STUB_H_
#define CANVAS_MAIN_STUB_H_

namespace Canvas
{
  namespace Main
  {
    typedef std::vector<Core::Region> QuerySet;

    class Stub
    {
      private:
        Core::Context* context;

      public:
        void execute(std::string command);
        void add(
            std::string registry,
            Core::Region region);
        QuerySet retrieve(
            std::string registry);
        QuerySet orderedRetrieve(
            std::string name,
            std::string orderBy);
        Stub();
    };

    void Stub::execute(
        std::string command)
    {
      Engine::execute(context, command);
      return;
    }

    void Stub::add(
        std::string name,
        Core::Region region)
    {
      Core::RegionsHub* regionsHub = context->getRegionsHub();
      regionsHub->add(name, region);
    }

    std::vector<Core::Region> Stub::retrieve(
        std::string name)
    {
      std::vector<std::string> variables, values;
      variables.push_back("source");
      values.push_back(name);
      Handler::Query* query = new Handler::QueryIntersect();

      query->setContext(context, variables, values);
      std::vector<Core::Region> result = query->operator()(
          Core::Region(-Constants::FLOAT_MAX, -Constants::FLOAT_MAX,
            Constants::FLOAT_MAX, Constants::FLOAT_MAX));

      return result;
    }

    std::vector<Core::Region> Stub::orderedRetrieve(
        std::string name,
        std::string orderBy)
    {
      std::vector<Core::Region> result = retrieve(name);
      if (orderBy == "minX") std::sort(result.begin(), result.end(),
          [](Core::Region& lhs, Core::Region& rhs) -> bool { return lhs.getX1() < rhs.getX1(); });
      else if (orderBy == "maxX") std::sort(result.begin(), result.end(),
          [](Core::Region& lhs, Core::Region& rhs) -> bool { return lhs.getX2() < rhs.getX2(); });
      else if (orderBy == "minY") std::sort(result.begin(), result.end(),
          [](Core::Region& lhs, Core::Region& rhs) -> bool { return  lhs.getY1() < rhs.getY1(); });
      else if (orderBy == "maxY") std::sort(result.begin(), result.end(),
          [](Core::Region& lhs, Core::Region& rhs) -> bool { return lhs.getY2() < rhs.getY2(); });
      return result;
    }

    Stub::Stub()
    {
      context = new Core::Context();
    }

  }
}

#endif
