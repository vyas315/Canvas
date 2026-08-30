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

#ifndef CANVAS_HANDLER_PROJECTIONSPLIT_H_
#define CANVAS_HANDLER_PROJECTIONSPLIT_H_

namespace Canvas
{
  namespace Handler
  {
    class SplitVerticalProjection : public Split
    {
      private:
        unsigned int anchorProjectionCount;
        unsigned int width;

        static bool compareRegions(
            Core::Region lhs,
            Core::Region rhs);
        void getWidth(
            std::vector<Core::Region> components);
        std::vector<unsigned int> getAnchorPositions(
            std::vector<Core::Region> components);
        std::vector<unsigned int> computeProjection(
            std::vector<Core::Region> components);
        std::vector<unsigned int> getColumnPositions(
            std::vector<Core::Region> components);

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        std::vector<Core::Region> operator()(
            std::vector<Core::Region> from,
            std::vector<Core::Region> to);
    };

    bool SplitVerticalProjection::compareRegions(
        Core::Region lhs,
        Core::Region rhs)
    {
      float verticalDiff = lhs(3) - rhs(3);
      float width = std::min(lhs(3) - lhs(1), rhs(3) - rhs(1));
      if (fabs(verticalDiff) <= 0.2 * width) return lhs(0) < rhs(0);
      return verticalDiff > 0.0;
    }

    void SplitVerticalProjection::getWidth(
        std::vector<Core::Region> components)
    {
      float maxX = -Constants::FLOAT_MAX;
      for (unsigned int index = 0; index < components.size(); index++)
        maxX = std::max(maxX, components[index](2));
      width = maxX;
      return;
    }

    std::vector<unsigned int> SplitVerticalProjection::getAnchorPositions(
        std::vector<Core::Region> components)
    {
      unsigned int index, xPos, revertIndex, flag = false;
      std::vector<unsigned int> projection(width + 1, 0);
      for (index = 0; index < components.size(); index++)
      {
        if (flag) break;
        for (xPos = components[index](0); xPos < components[index](2); xPos++)
        {
          if (projection[xPos] + 1 <= anchorProjectionCount) projection[xPos] += 1;
          else
          {
            revertIndex = xPos;
            for ( xPos = components[index](0); xPos < revertIndex; xPos++)
              projection[xPos] -= 1;
            flag = true;
            break;
          }
        }
      }

      std::vector<unsigned int> anchorPositions;
      anchorPositions.push_back(0);
      flag = false;
      for (xPos = 0; xPos < width + 1; xPos++)
      {
        if (!flag && !projection[xPos]) continue;
        else if (!flag && projection[xPos])
          { flag = true; anchorPositions.push_back(xPos); }
        else if (flag && projection[xPos]) continue;
        else {flag = false; anchorPositions.push_back(xPos); }
      }
      if (flag == true) anchorPositions.push_back(width);
      anchorPositions.push_back(width);

      return anchorPositions;
    }

    std::vector<unsigned int> SplitVerticalProjection::computeProjection(
        std::vector<Core::Region> components)
    {
      unsigned int index, xPos;
      std::vector<unsigned int> projection(width, 0);
      for (index = 0; index < components.size(); index++)
        for (xPos = components[index](0); xPos < components[index](2); xPos++)
          projection[xPos] += 1;

      return projection;
    }

    std::vector<unsigned int> SplitVerticalProjection::getColumnPositions(
        std::vector<Core::Region> components)
    {
      getWidth(components);
      std::vector<unsigned int> anchorPositions = getAnchorPositions(components);
      std::vector<unsigned int> projection = computeProjection(components);

      unsigned int index, startPos, endPos, minProjection, minPos, xPos;
      std::vector<unsigned int> columnPositions;
      for (index = 0; index < anchorPositions.size(); index += 2)
      {
        startPos = anchorPositions[index];
        endPos = anchorPositions[index + 1];
        minProjection = projection[startPos];
        minPos = startPos;

        for (xPos = startPos; xPos <= endPos; xPos++)
        {
          if (minProjection > projection[xPos])
            { minProjection = projection[xPos]; minPos = xPos; }
        }
        columnPositions.push_back(minPos);
      }
      return columnPositions;
    }

    void SplitVerticalProjection::setParameter(
        std::string parameter,
        std::string value)
    {
      if (parameter == "anchorCount") anchorProjectionCount = std::stoi(value);
      return;
    }

    std::vector<Core::Region> SplitVerticalProjection::operator()(
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

      std::sort(to.begin(), to.end(), compareRegions);
      std::vector<unsigned int> columnPositions = getColumnPositions(to);
      if (columnPositions[columnPositions.size() - 1] < maxX)
        columnPositions[columnPositions.size() - 1] = maxX;

      std::vector<Core::Region> result;
      float currentStart = minX;
      for (unsigned int index = 0; index < columnPositions.size(); index++)
      {
        if (columnPositions[index] >= maxX) break;
        if ((columnPositions[index] - currentStart) > constants::SPLIT_TOLERANCE)
          result.push_back(Core::Region(currentStart, minY, columnPositions[index], maxY, ""));
        currentStart = columnPositions[index];
      }
      if (currentStart < maxX)
        result.push_back(Core::Region(currentStart, minY, maxX, maxY, ""));
      return result;
    }

  }
}

#endif
