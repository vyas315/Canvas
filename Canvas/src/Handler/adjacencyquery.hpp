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

#ifndef CANVAS_HANDLER_ADJACENCYQUERY_H_
#define CANVAS_HANDLER_ADJACENCYQUERY_H_

namespace Canvas
{
  namespace Handler
  {
    class QueryAdj : public Query
    {
      protected:
        Core::RegionsHub* regionsHub;
        std::string source;
        float maxDistance;
        float trimVertical;
        float trimHorizontal;

      public:
        void setContext(
            Core::Context* context,
            std::vector<std::string> variable,
            std::vector<std::string> value);
        void setParameter(
            std::string parameter,
            std::string value);
        QueryAdj();
    };

    class QueryAdjLeft : public QueryAdj
    {
      public:
        std::vector<Core::Region> operator()(
            Core::Region queryRegion);
    };

    class QueryAdjRight : public QueryAdj
    {
      public:
        std::vector<Core::Region> operator()(
            Core::Region queryRegion);
    };

    class QueryAdjTop : public QueryAdj
    {
      public:
        std::vector<Core::Region> operator()(
            Core::Region queryRegion);
    };

    class QueryAdjBottom : public QueryAdj
    {
      public:
        std::vector<Core::Region> operator()(
            Core::Region queryRegion);
    };

    void QueryAdj::setContext(
        Core::Context* context,
        std::vector<std::string> variables,
        std::vector<std::string> values)
    {
      this->context = context;
      this->regionsHub = this->context->getRegionsHub();

      for (unsigned int index = 0; index < variables.size(); index++)
        if (variables[index] == "source") source = values[index];

      this->regionsHub->setActiveRegistry(source);
      return;
    }

    void QueryAdj::setParameter(
        std::string parameter,
        std::string value)
    {
      if (parameter == "maxDistance") maxDistance = std::stof(value);
      else if (parameter == "trimVertical") trimVertical = std::stof(value);
      else if (parameter == "trimHorizontal") trimHorizontal = std::stof(value);
      return;
    }

    QueryAdj::QueryAdj()
    {
      maxDistance = Constants::FLOAT_MAX;
      trimVertical = 0;
      trimHorizontal = 0;
    }

    std::vector<Core::Region>
        QueryAdjLeft::operator()(
        Core::Region queryRegion)
    {
      float verticalOffset = constants::EPSILON;
      if (trimVertical > constants::EPSILON)
          verticalOffset = (queryRegion(3) - queryRegion(1)) * trimVertical;

      Core::Region searchRegion(queryRegion(0) - maxDistance,
          queryRegion(1) + verticalOffset, queryRegion(0) - constants::EPSILON,
          queryRegion(3) - verticalOffset);
      return regionsHub->fringeQuery(searchRegion, Core::constants::RIGHT);
    }

    std::vector<Core::Region>
        QueryAdjRight::operator()(
        Core::Region queryRegion)
    {
      float verticalOffset = constants::EPSILON;
      if (trimVertical > constants::EPSILON)
          verticalOffset = (queryRegion(3) - queryRegion(1)) * trimVertical;

      Core::Region searchRegion(queryRegion(2) + constants::EPSILON,
          queryRegion(1) + verticalOffset, queryRegion(2) + maxDistance,
          queryRegion(3) - verticalOffset);
      return regionsHub->fringeQuery(searchRegion, Core::constants::LEFT);
    }

    std::vector<Core::Region>
        QueryAdjTop::operator()(
        Core::Region queryRegion)
    {
      float horizontalOffset = constants::EPSILON;
      if (trimHorizontal > constants::EPSILON)
          horizontalOffset = (queryRegion(2) - queryRegion(0)) * trimHorizontal;

      Core::Region searchRegion(queryRegion(0) + horizontalOffset,
          queryRegion(3) + constants::EPSILON, queryRegion(2) - horizontalOffset,
          queryRegion(3) + maxDistance);
      return regionsHub->fringeQuery(searchRegion, Core::constants::BOTTOM);
    }

    std::vector<Core::Region>
        QueryAdjBottom::operator()(
        Core::Region queryRegion)
    {
      float horizontalOffset = constants::EPSILON;
      if (trimHorizontal > constants::EPSILON)
          horizontalOffset = (queryRegion(2) - queryRegion(0)) * trimHorizontal;

      Core::Region searchRegion(queryRegion(0) + horizontalOffset,
          queryRegion(1) - maxDistance, queryRegion(2) - horizontalOffset,
          queryRegion(1) - constants::EPSILON);
      return regionsHub->fringeQuery(searchRegion, Core::constants::TOP);
    }

  }
}

#endif
