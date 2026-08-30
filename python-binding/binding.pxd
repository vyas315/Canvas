# Copyright 2018-2026 Vedavyas Chigurupati
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from libcpp.vector cimport vector

cdef extern from "string" namespace "std":
    cdef cppclass string:
        char* c_str()


cdef extern from "Canvas/Core" namespace "Canvas::Core":
  cdef cppclass Region:
    Region() except +
    Region(float X1, float Y1, float X2, float Y2) except +
    Region(float X1, float Y1, float X2, float Y2, string value) except +
    Region(float X1, float Y1, float X2, float Y2, string value, unsigned int id) except +

    string getValue()
    unsigned int getId()
    float getX1()
    float getY1()
    float getX2()
    float getY2()
  

cdef extern from "Canvas/Main" namespace "Canvas::Main":
  cdef cppclass Stub:
    Stub() except +

    void execute(string command)
    void add(string registry, Region region)
    vector[Region] retrieve(string registry)
    vector[Region] orderedRetrieve(string name, string orderBy)
