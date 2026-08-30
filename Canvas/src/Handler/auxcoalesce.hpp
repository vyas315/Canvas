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

#ifndef CANVAS_HANDLER_AUXCOALESCE_H_
#define CANVAS_HANDLER_AUXCOALESCE_H_

namespace Canvas
{
  namespace Handler
  {
    class CoalesceAreaValue : public Coalesce
    {
      private:
        static bool compareRegions(
            Core::Region lhs,
            Core::Region rhs);
        std::string delimiter;
        std::string order;

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        Core::Region operator()(
            std::vector<Core::Region> from,
            std::vector<Core::Region> to);
        CoalesceAreaValue();
    };

    bool CoalesceAreaValue::compareRegions(
        Core::Region lhs,
        Core::Region rhs)
    {
        float verticalDiff = lhs(3) - rhs(3);
        float width = std::min(lhs(3) - lhs(1), rhs(3) - rhs(1));
        if (fabs(verticalDiff) <= 0.2 * width) return lhs(0) < rhs(0);
        return verticalDiff > 0.0;
    }

    void CoalesceAreaValue::setParameter(
        std::string parameter,
        std::string value)
    {
      if (parameter == "delimiter") delimiter = value;
      else if (parameter == "order") order = value;
    }

    Core::Region CoalesceAreaValue::operator()(
        std::vector<Core::Region> from,
        std::vector<Core::Region> to)
    {
      float minX = Constants::FLOAT_MAX;
      float minY = Constants::FLOAT_MAX;
      float maxX = -Constants::FLOAT_MAX;
      float maxY = -Constants::FLOAT_MAX;

      for (unsigned int index = 0; index < from.size(); index++)
      {
        minX = std::min(minX, from[index](0));
        minY = std::min(minY, from[index](1));
        maxX = std::max(maxX, from[index](2));
        maxY = std::max(maxY, from[index](3));
      }

      std::string value("");

      if (order == "y")  std::sort(to.begin(), to.end(),
          [](Core::Region lhs, Core::Region rhs) { return lhs(1) > rhs(1); });
      else std::sort(to.begin(), to.end(), compareRegions);

      for (unsigned int index = 0; index < to.size(); index++)
      {
        value += to[index].getValue();
        if (index < to.size() - 1) value += delimiter;
      }

      if (from.size() > 1) return Core::Region(minX, minY, maxX, maxY, value);
      else return Core::Region(minX, minY, maxX, maxY, value, from[0].getId());
    }

    CoalesceAreaValue::CoalesceAreaValue() { delimiter = ""; order = "x"; }

    class CoalesceArea : public Coalesce
    {
      private:
        std::string delimiter;

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        Core::Region operator()(
            std::vector<Core::Region> from,
            std::vector<Core::Region> to);
        CoalesceArea();
    };

    void CoalesceArea::setParameter(
        std::string parameter,
        std::string value)
    { return; }

    Core::Region CoalesceArea::operator()(
        std::vector<Core::Region> from,
        std::vector<Core::Region> to)
    {
      float minX = Constants::FLOAT_MAX;
      float minY = Constants::FLOAT_MAX;
      float maxX = -Constants::FLOAT_MAX;
      float maxY = -Constants::FLOAT_MAX;

      for (unsigned int index = 0; index < from.size(); index++)
      {
        minX = std::min(minX, from[index](0));
        minY = std::min(minY, from[index](1));
        maxX = std::max(maxX, from[index](2));
        maxY = std::max(maxY, from[index](3));
      }

      for (unsigned int index = 0; index < to.size(); index++)
      {
        minX = std::min(minX, to[index](0));
        minY = std::min(minY, to[index](1));
        maxX = std::max(maxX, to[index](2));
        maxY = std::max(maxY, to[index](3));
      }

      return Core::Region(minX, minY, maxX, maxY, "");
    }

    CoalesceArea::CoalesceArea() {}

  }
}

#endif
