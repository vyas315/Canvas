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

#ifndef CANVAS_CORE_REGIONREGISTRYINTERVAL_H_
#define CANVAS_CORE_REGIONREGISTRYINTERVAL_H_

namespace Canvas
{
  namespace Core
  {
    struct IntervalNode
    {
      Region region;

      float minX;
      float minY;
      float maxX;
      float maxY;

      IntervalNode* C1;
      IntervalNode* C2;
      IntervalNode* C3;
      IntervalNode* C4;
      IntervalNode(Region& region_);
    };

    struct IndexNode
    {
      unsigned int key;

      IndexNode* left;
      IndexNode* right;
      IntervalNode* dataNode;
      IndexNode(
          unsigned int key,
          IntervalNode* dataNode);
    };

    class RegionRegistryInterval : public RegionRegistry
    {
      IntervalNode* root;
      IndexNode* indexRoot;

      void addIndex(
          Region region,
          IntervalNode* dataNode);
      Region getRegionsById(
          IndexNode* node,
          unsigned int id);

      void intersectQueryRecurse(
          std::vector<Region>& result,
          IntervalNode* node,
          Region queryRegion);

      bool checkFringePrune(
          IntervalNode* node,
          Region queryRegion,
          enum constants::FringeType fringeType);
      bool checkFringeUpdate(
          Region referance,
          Region query,
          enum constants::FringeType fringeType);
      void fringeQueryRecurse(
          IntervalNode*& candidate,
          IntervalNode* node,
          Region queryRegion,
          enum constants::FringeType fringeType);

      std::vector<Region> intersectQueryImpl(
          Region queryRegion);
      std::vector<Region> fringeQueryImpl(
          Region queryRegion,
          enum constants::FringeType);
      void printImpl(IntervalNode* node);

    public:
      RegionRegistryInterval();
      std::vector<Region> intersectQuery(
          Region queryRegion);
      std::vector<Region> fringeQuery(
          Region queryRegion,
          enum constants::FringeType);
      void add(Region region);
      std::vector<Region> getRegionsByIds(
          std::vector<unsigned int> ids);
      void print();
    };

    IntervalNode::IntervalNode(
        Region& region_) : region(region_)
    {
      minX = region(0);
      minY = region(1);
      maxX = region(2);
      maxY = region(3);

      C1 = NULL;
      C2 = NULL;
      C3 = NULL;
      C4 = NULL;
    }

    IndexNode::IndexNode(
        unsigned int key,
        IntervalNode* dataNode)
    {
      this->key = key;
      this->left = NULL;
      this->right = NULL;
      this->dataNode = dataNode;
    }

    void RegionRegistryInterval::addIndex(
        Region region,
        IntervalNode* dataNode)
    {
      IndexNode** node = &indexRoot;
      while (true)
      {
        if (*node == NULL)
        { *node = new IndexNode(region.getId(), dataNode); return; }
        if ((*node)->key > region.getId()) node = &(*node)->right;
        else node = &(*node)->left;
      }
      return;
    }

    Region RegionRegistryInterval::getRegionsById(
        IndexNode* node,
        unsigned int id)
    {
      while (true)
      {
        if (node == NULL) printf("ERROR : id not present \n");
        if (node->key == id) return node->dataNode->region;
        if (node->key > id) node = node->right;
        else node = node->left;
      }
    }

    std::vector<Region>
        RegionRegistryInterval::getRegionsByIds(
        std::vector<unsigned int> ids)
    {
      std::vector<Region> result;
      for (unsigned int index = 0; index < ids.size(); index++)
        result.push_back(getRegionsById(indexRoot, ids[index]));
      return result;
    }

    constants::IntervalQuadrant compare(
        IntervalNode* node,
        Region region)
    {
      if (region(0) >= node->region(0) && region(1) >= node->region(1))
        return constants::Q1;
      else if (region(0) < node->region(0) && region(1) >= node->region(1))
        return constants::Q2;
      else if (region(0) < node->region(0) && region(1) < node->region(1))
        return constants::Q3;
      else return constants::Q4;
    }

    bool checkOverlap(
        IntervalNode* node,
        Region region)
    {
      if (node == NULL) return false;
      if (node->minX > region(2) || region(0) > node->maxX) return false;
      if (node->minY > region(3) || region(1) > node->maxY) return false;
      return true;
    }

    void RegionRegistryInterval::add(
        Region region)
    {
      IntervalNode** node = &root;
      while (true)
      {
        if (*node == NULL)
        {
          *node = new IntervalNode(region);
          addIndex(region, *node);
          break;
        }

        enum constants::IntervalQuadrant quadrant;
        quadrant = compare(*node, region);

        (*node)->minX = std::min((*node)->minX, region(0));
        (*node)->minY = std::min((*node)->minY, region(1));
        (*node)->maxX = std::max((*node)->maxX, region(2));
        (*node)->maxY = std::max((*node)->maxY, region(3));

        if (quadrant == constants::Q1) node = &(*node)->C1;
        else if (quadrant == constants::Q2) node = &(*node)->C2;
        else if (quadrant == constants::Q3) node = &(*node)->C3;
        else if (quadrant == constants::Q4) node = &(*node)->C4;
      }
      return;
    }

    void RegionRegistryInterval::intersectQueryRecurse(
        std::vector<Region>& result,
        IntervalNode* node,
        Region queryRegion)
    {
      if (node == NULL) return;
      if (!checkOverlap(node, queryRegion)) return;

      if (checkOverlap(node->region, queryRegion))
        result.push_back(node->region);
      intersectQueryRecurse(result, node->C1, queryRegion);
      intersectQueryRecurse(result, node->C2, queryRegion);
      intersectQueryRecurse(result, node->C3, queryRegion);
      intersectQueryRecurse(result, node->C4, queryRegion);
      return;
    }

    std::vector<Region>
        RegionRegistryInterval::intersectQueryImpl(
        Region queryRegion)
    {
      std::vector<Region> result;
      intersectQueryRecurse(result, root, queryRegion);
      return result;
    }

    bool RegionRegistryInterval::checkFringePrune(
        IntervalNode* node,
        Region region,
        constants::FringeType fringeType)
    {
      if (fringeType == constants::LEFT)
        return node->minX > region(0);
      else if (fringeType == constants::RIGHT)
        return node->maxX < region(2);
      else if (fringeType == constants::BOTTOM)
        return node->minY > region(1);
      else if (fringeType == constants::TOP)
        return node->maxY < region(3);
    }

    bool RegionRegistryInterval::checkFringeUpdate(
        Region referance,
        Region query,
        constants::FringeType fringeType)
    {
      if (fringeType == constants::LEFT)
        return query(0) <= referance(0);
      else if (fringeType == constants::RIGHT)
        return query(2) >= referance(2);
      else if (fringeType == constants::BOTTOM)
        return query(1) <= referance(1);
      else if (fringeType == constants::TOP)
        return query(3) >= referance(3);
    }

    void RegionRegistryInterval::fringeQueryRecurse(
        IntervalNode*& candidate,
        IntervalNode* node,
        Region queryRegion,
        enum constants::FringeType fringeType)
    {
      if (node == NULL) return;
      if (!checkOverlap(node, queryRegion)) return;
      if (candidate != NULL && checkFringePrune(node, candidate->region,
          fringeType)) return;

      if (checkOverlap(node->region, queryRegion) &&
          (candidate == NULL || checkFringeUpdate(candidate->region, node->region,
          fringeType)))
        candidate = node;

      fringeQueryRecurse(candidate, node->C1, queryRegion, fringeType);
      fringeQueryRecurse(candidate, node->C2, queryRegion, fringeType);
      fringeQueryRecurse(candidate, node->C3, queryRegion, fringeType);
      fringeQueryRecurse(candidate, node->C4, queryRegion, fringeType);
      return;
    }

    std::vector<Region>
        RegionRegistryInterval::fringeQueryImpl(
        Region queryRegion,
        constants::FringeType fringeType)
    {
      std::vector<Region> result;
      if (!checkOverlap(root, queryRegion)) return result;

      IntervalNode* candidate = NULL;
      fringeQueryRecurse(candidate, root, queryRegion, fringeType);
      if (candidate != NULL)
        result.push_back(candidate->region);
      return result;
    }

    void RegionRegistryInterval::printImpl(
        IntervalNode* node)
    {
      if (node == NULL) return;
      printf("%s \n", (node->region.getValue()).c_str());
      printf("%f %f %f %f\n", node->minX, node->minY, node->maxX, node->maxY);
      printImpl(node->C1);
      printImpl(node->C2);
      printImpl(node->C3);
      printImpl(node->C4);
      return;
    }

    std::vector<Region>
        RegionRegistryInterval::fringeQuery(
        Region queryRegion,
        constants::FringeType fringeType)
    { return fringeQueryImpl(queryRegion, fringeType); }

    std::vector<Region>
        RegionRegistryInterval::intersectQuery(
        Region queryRegion)
    { return intersectQueryImpl(queryRegion); }

    void RegionRegistryInterval::print()
    { printImpl(root); return; }

    RegionRegistryInterval::RegionRegistryInterval()
    { root = NULL; indexRoot = NULL; }

  }
}

#endif
