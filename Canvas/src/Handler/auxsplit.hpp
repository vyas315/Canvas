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

#ifndef CANVAS_HANDLER_AUXSPLIT_H_
#define CANVAS_HANDLER_AUXSPLIT_H_

namespace Canvas
{
  namespace Handler
  {
    class SplitLeft : public Split
    {
      private:
        static bool compareRegions(
            Core::Region lhs,
            Core::Region rhs);

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        std::vector<Core::Region> operator()(
            std::vector<Core::Region> from,
            std::vector<Core::Region> to);
    };

    bool SplitLeft::compareRegions(
        Core::Region lhs,
        Core::Region rhs)
    {
      return lhs((unsigned int) 0) < rhs((unsigned int) 0);
    }

    void SplitLeft::setParameter(
        std::string parameter,
        std::string value)
    {}

    std::vector<Core::Region> SplitLeft::operator()(
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

      std::vector<Core::Region> result;
      std::sort(to.begin(), to.end(), compareRegions);
      float currentStart = minX;
      for (unsigned int index = 0; index < to.size(); index++)
      {
        if (to[index](2) >= maxX) break;
        if ((to[index](2) - currentStart) > constants::SPLIT_TOLERANCE)
          result.push_back(Core::Region(currentStart, minY, to[index](2), maxY, ""));
        currentStart = to[index](2);
      }
      if (currentStart < maxX)
        result.push_back(Core::Region(currentStart, minY, maxX, maxY, ""));
      return result;
    }

    class SplitTop : public Split
    {
      private:
        static bool compareRegions(
            Core::Region lhs,
            Core::Region rhs);

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        std::vector<Core::Region> operator()(
            std::vector<Core::Region> from,
            std::vector<Core::Region> to);
    };

    bool SplitTop::compareRegions(
        Core::Region lhs,
        Core::Region rhs)
    {
      return lhs(3) > rhs(3);
    }

    void SplitTop::setParameter(
        std::string parameter,
        std::string value)
    {}

    std::vector<Core::Region> SplitTop::operator()(
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

      std::vector<Core::Region> result;
      std::sort(to.begin(), to.end(), compareRegions);

      float currentStart = maxY;
      for (unsigned int index = 1; index < to.size(); index++)
      {
        if ((currentStart - to[index](3)) > constants::SPLIT_TOLERANCE)
          result.push_back(Core::Region(minX, to[index](3), maxX, currentStart, ""));
        currentStart = to[index](3);
      }
      if (currentStart > minY)
        result.push_back(Core::Region(minX, minY, maxX, currentStart, ""));
      return result;
    }

  }
}

#endif
