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

#ifndef CANVAS_HANDLER_AUXQUERY_H_
#define CANVAS_HANDLER_AUXQUERY_H_

namespace Canvas
{
  namespace Handler
  {
    class QueryIntersect : public Query
    {
      private:
        Core::RegionsHub* regionsHub;
        std::string source;
        float minOverlap;
        float getOverlap(
            Core::Region queryRegion,
            Core::Region candidateRegion);

      public:
        void setContext(
            Core::Context* context,
            std::vector<std::string> variables,
            std::vector<std::string> values);
        void setParameter(
            std::string parameter,
            std::string value);
        std::vector<Core::Region> operator()(
            Core::Region region);
        QueryIntersect();
    };

    void QueryIntersect::setContext(
        Core::Context* context,
        std::vector<std::string> variables,
        std::vector<std::string> values)
    {
      this->context = context;
      this->regionsHub = this->context->getRegionsHub();

      for (unsigned int index = 0; index < variables.size(); index++)
        if (variables[index] == "source") source = values[index];

      return;
    }

    void QueryIntersect::setParameter(
        std::string parameter,
        std::string value)
    {
      if (parameter == "minOverlap") minOverlap = std::stof(value);
    }

    float QueryIntersect::getOverlap(
        Core::Region queryRegion,
        Core::Region candidateRegion)
    {
      float overlap = 0.0;
      overlap = std::max(0.0f, std::min(queryRegion(2), candidateRegion(2)) -
                         std::max(queryRegion(0), candidateRegion(0))) *
                std::max(0.0f, std::min(queryRegion(3), candidateRegion(3)) -
                         std::max(queryRegion(1), candidateRegion(1)));
      return std::max(0.0f, overlap / ((candidateRegion(2) - candidateRegion(0)) *
                        (candidateRegion(3) - candidateRegion(1))));
    }

    std::vector<Core::Region> QueryIntersect::operator()(
        Core::Region queryRegion)
    {
      std::vector<Core::Region> candidateRegions = regionsHub->intersectQuery(
          source, queryRegion);
      std::vector<Core::Region> result;
      for (unsigned int index = 0; index < candidateRegions.size(); index++)
      {
        if (getOverlap(queryRegion, candidateRegions[index]) > minOverlap)
          result.push_back(candidateRegions[index]);
      }
      return result;
    }

    QueryIntersect::QueryIntersect() { minOverlap = 0.0; }

  }
}

#endif
