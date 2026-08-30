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

#ifndef CANVAS_HANDLER_MERGE_H_
#define CANVAS_HANDLER_MERGE_H_

namespace Canvas
{
  namespace Handler
  {
    class ReduceMerge : public Reduce
    {
      private:
        std::string delimiter;

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        Core::Region operator()(
            std::vector<Core::Region> regions);
        ReduceMerge();
    };

    class ReduceLeftRightConcat : public Reduce
    {
      private:
        std::string delimiter;
        static bool compareRegions(
            Core::Region lhs,
            Core::Region rhs);

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        Core::Region operator()(
            std::vector<Core::Region> regions);
        ReduceLeftRightConcat();
    };

    class ReduceTopBottomConcat : public Reduce
    {
      private:
        std::string delimiter;
        static bool compareRegions(
            Core::Region lhs,
            Core::Region rhs);

      public:
        void setParameter(
            std::string parameter,
            std::string value);
        Core::Region operator()(
            std::vector<Core::Region> regions);
        ReduceTopBottomConcat();
    };

    void ReduceMerge::setParameter(
        std::string parameter,
        std::string value)
    { if (parameter == "delimiter") delimiter = value; }

    Core::Region ReduceMerge::operator()(
        std::vector<Core::Region> regions)
    {
      float minX = Constants::FLOAT_MAX;
      float minY = Constants::FLOAT_MAX;
      float maxX = -Constants::FLOAT_MAX;
      float maxY = -Constants::FLOAT_MAX;
      std::string value("");

      for (unsigned int index = 0; index < regions.size(); index++)
      {
        minX = std::min(minX, regions[index](0));
        minY = std::min(minY, regions[index](1));
        maxX = std::max(maxX, regions[index](2));
        maxY = std::max(maxY, regions[index](3));

        value += regions[index].getValue();
        if (index < regions.size() - 1) value += delimiter;
      }

      return Core::Region(minX, minY, maxX, maxY, value);
    }

    ReduceMerge::ReduceMerge() { delimiter = ""; }

    bool ReduceLeftRightConcat::compareRegions(
        Core::Region lhs,
        Core::Region rhs)
    {
      return lhs((unsigned int) 0) < rhs((unsigned int) 0);
    }

    void ReduceLeftRightConcat::setParameter(
        std::string parameter,
        std::string value)
    { if (parameter == "delimiter") delimiter = value; }

    Core::Region ReduceLeftRightConcat::operator()(
        std::vector<Core::Region> regions)
    {
      std::sort(regions.begin(), regions.end(), compareRegions);

      float minX = Constants::FLOAT_MAX;
      float minY = Constants::FLOAT_MAX;
      float maxX = -Constants::FLOAT_MAX;
      float maxY = -Constants::FLOAT_MAX;
      std::string value("");

      for (unsigned int index = 0; index < regions.size(); index++)
      {
        minX = std::min(minX, regions[index](0));
        minY = std::min(minY, regions[index](1));
        maxX = std::max(maxX, regions[index](2));
        maxY = std::max(maxY, regions[index](3));

        value += regions[index].getValue();
        if (index < regions.size() - 1) value += delimiter;
      }

      return Core::Region(minX, minY, maxX, maxY, value);
    }

    ReduceLeftRightConcat::ReduceLeftRightConcat() {delimiter = ""; }

    bool ReduceTopBottomConcat::compareRegions(
        Core::Region lhs,
        Core::Region rhs)
    {
      return lhs((unsigned int) 3) > rhs((unsigned int) 3);
    }

    void ReduceTopBottomConcat::setParameter(
        std::string parameter,
        std::string value)
    { if (parameter == "delimiter") delimiter = value; }

    Core::Region ReduceTopBottomConcat::operator()(
        std::vector<Core::Region> regions)
    {
      std::sort(regions.begin(), regions.end(), compareRegions);

      float minX = Constants::FLOAT_MAX;
      float minY = Constants::FLOAT_MAX;
      float maxX = -Constants::FLOAT_MAX;
      float maxY = -Constants::FLOAT_MAX;
      std::string value("");

      for (unsigned int index = 0; index < regions.size(); index++)
      {
        minX = std::min(minX, regions[index](0));
        minY = std::min(minY, regions[index](1));
        maxX = std::max(maxX, regions[index](2));
        maxY = std::max(maxY, regions[index](3));

        value += regions[index].getValue();
        if (index < regions.size() - 1) value += delimiter;
      }

      return Core::Region(minX, minY, maxX, maxY, value);
    }

    ReduceTopBottomConcat::ReduceTopBottomConcat() {delimiter = ""; }

  }
}

#endif
