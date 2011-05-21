# -*- coding: utf8 -*-

'''

@author Andreas Würl

'''

import subprocess
import re
import pyproj

class Point(object):

  __geod = pyproj.Geod(ellps='WGS84', units='m')

  __whitespaceRe = re.compile('\s+')

  def __init__(self, x, y):
    self.x = x
    self.y = y

  def __invgeod(self, other):
    return Point.__geod.inv(self.x, self.y, other.x, other.y)

  def distance(self, other):
    return self.__invgeod(other)[2]

  def azimuth(self, other):
    return self.__invgeod(other)[0]
