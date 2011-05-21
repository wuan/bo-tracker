# -*- coding: utf8 -*-

'''

@author Andreas Würl

'''

import subprocess
import re

class Point(object):

  __whitespaceRe = re.compile('\s+')

  def __init__(self, x, y):
    self.x = x
    self.y = y

  def __invgeod(self, other):
    pipe = subprocess.Popen(['invgeod', '+ellps=WGS84', '+units=m','-f','%.5f'], stdin = subprocess.PIPE, stdout = subprocess.PIPE)
    (output, error) = pipe.communicate("%f %f %f %f" %(self.y, self.x, other.y, other.x))
    return Point.__whitespaceRe.split(output.strip())

  def distance(self, other):
    return float(self.__invgeod(other)[2])

  def azimuth(self, other):
    return float(self.__invgeod(other)[0])
