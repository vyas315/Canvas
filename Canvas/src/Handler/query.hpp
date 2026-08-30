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

#ifndef CANVAS_HANDLER_QUERY_H_
#define CANVAS_HANDLER_QUERY_H_

namespace Canvas
{
  namespace Handler
  {
    class Query
    {
      protected:
        Core::Context* context;

      public:
        virtual void setContext(
            Core::Context* context,
            std::vector<std::string> variables,
            std::vector<std::string> values) = 0;
        virtual void setParameter(
            std::string parameter,
            std::string value) = 0;
        virtual std::vector<Core::Region> operator()(
            Core::Region queryRegion) = 0;
    };
  }
}

#endif
