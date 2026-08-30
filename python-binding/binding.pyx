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

from binding cimport string
from binding cimport Stub as CStub
from binding cimport Region as CRegion

cdef class Region:
  cdef CRegion region

  def __cinit__(self, float X1, float Y1, float X2, float Y2, value='', id=None):
    if (id is not None):
      self.region = CRegion(X1, Y1, X2, Y2, value.encode('utf-8'), id)
    self.region = CRegion(X1, Y1, X2, Y2, value.encode('utf-8'))
  
  def get_x1(self):
    return self.region.getX1()

  def get_y1(self):
    return self.region.getY1()

  def get_x2(self):
    return self.region.getX2()

  def get_y2(self):
    return self.region.getY2()

  def get_value(self):
    return self.region.getValue().decode()

  def get_id(self):
    return self.region.getId()
  
  property x1:
    def __get__(self):
      return self.get_x1()
  
  property y1:
    def __get__(self):
      return self.get_y1()

  property x2:
    def __get__(self):
      return self.get_x2()
  
  property y2:
    def __get__(self):
      return self.get_y2()
  
  property value:
    def __get__(self):
      return self.get_value()
  
  property id:
    def __get__(self):
      return self.get_id()


cdef class Stub:
  cdef CStub stub
  cdef CRegion region
  cdef vector[CRegion] query_set

  def __cinit__(self):
    self.stub = CStub()
  
  def add(self, registry, Region region):
    self.stub.add(registry.encode('utf-8'), region.region)
  
  def execute(self, command):
    self.stub.execute(command.encode('utf-8'))
  
  def retrieve(self, registry):
    query_set = self.stub.retrieve(registry.encode('utf-8'))
    return [Region(region.getX1(), region.getY1(), region.getX2(), region.getY2(), region.getValue().decode()) for region in query_set]

  def ordered_retrieve(self, registry, order_by):
    query_set = self.stub.orderedRetrieve(registry.encode('utf-8'), order_by.encode('utf-8'))
    return [Region(region.getX1(), region.getY1(), region.getX2(), region.getY2(), region.getValue().decode()) for region in query_set]
    
