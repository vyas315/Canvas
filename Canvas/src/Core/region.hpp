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

#ifndef CANVAS_CORE_REGION_H_
#define CANVAS_CORE_REGION_H_

namespace Canvas
{
  namespace Core
  {
    class Region
    {
      private:
        static unsigned int counter;
        unsigned int id;
        std::string value;
        float* bbox;

      public:
        Region();
        Region(
            float X1,
            float Y1,
            float X2,
            float Y2,
            std::string value);
        Region(
            float X1,
            float Y1,
            float X2,
            float Y2);
        Region(
          float X1,
          float Y1,
          float X2,
          float Y2,
          std::string value,
          unsigned int id);

        std::string getValue();
        unsigned int getId();
        float getX1();
        float getY1();
        float getX2();
        float getY2();
        float operator()(
            unsigned int index);
        bool operator==(
            Region rhs);
      };

    Region::Region() {}
    Region::Region(
        float X1,
        float Y1,
        float X2,
        float Y2,
        std::string value)
    {
      this->value = value;
      id = counter++;
      bbox = (float*) malloc(4 * sizeof(bbox));
      bbox[0] = X1;
      bbox[1] = Y1;
      bbox[2] = X2;
      bbox[3] = Y2;
    }

    Region::Region(
        float X1,
        float Y1,
        float X2,
        float Y2)
    {
      id = UINT_MAX;
      bbox = (float*) malloc(4 * sizeof(bbox));
      bbox[0] = X1;
      bbox[1] = Y1;
      bbox[2] = X2;
      bbox[3] = Y2;
    }

    Region::Region(
        float X1,
        float Y1,
        float X2,
        float Y2,
        std::string value,
        unsigned int id)
    {
      this->value = value;
      this->id = id;
      bbox = (float*) malloc(4 * sizeof(bbox));
      bbox[0] = X1;
      bbox[1] = Y1;
      bbox[2] = X2;
      bbox[3] = Y2;
    }

    std::string Region::getValue() { return value; }

    unsigned int Region::getId() { return id; }

    float Region::operator()(unsigned int index) { return bbox[index]; }

    float Region::getX1() { return bbox[0]; }

    float Region::getY1() { return bbox[1]; }

    float Region::getX2() { return bbox[2]; }

    float Region::getY2() { return bbox[3]; }

    bool Region::operator==(Region rhs) { return id == rhs.id; }

    bool checkOverlap(
        Region referance,
        Region query)
    {
      if (referance(0) > query(2) || query(0) > referance(2)) return false;
      if (referance(1) > query(3) || query(1) > referance(3)) return false;
      return true;
    }

    unsigned int Region::counter = 0;
  }
}

#endif
