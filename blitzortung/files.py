# -*- coding: utf8 -*-

'''

@author Andreas Würl

'''

import os
import glob
import datetime

class RawFiles(object):

  def __init__(self, rawPath):
    rawFileNames = glob.glob(os.path.join(rawPath, '*.bor'))

    rawFileNames.sort()

    self.rawFiles = {}

    for rawFileName in rawFileNames:
      date = datetime.datetime.strptime(rawFileName[-12:-4], '%Y%m%d').date()
      if not self.rawFiles.has_key(date):
        self.rawFiles[date] = rawFileName
      else:
        raise Exception("ERROR: double date! " + rawFileName + " vs. " + self.rawFiles[date])

  def get(self, date):
    if self.rawFiles.has_key(date):
      return self.rawFiles[date]
    else:
      raise Exception("no file for date "+date.strftime('%Y-%m-%d'))


class Data(object):

  def __init__(self, files, time):
    self.files = files
    self.time = time

  def get(self):
    start = self.time.getStartTime()
    starttime = start.strftime("%H%M")
    startdate = start.strftime("%Y%m%d")

    end = self.time.getEndMinute()
    endtime = end.strftime("%H%M")

    try:
      rawFile = files.get(start.date())

      data = subprocess.Popen(['blitzortung-data','-i', rawFile, '-s', starttime, '-e', endtime], stdout=subprocess.PIPE)

      (output, error) = data.communicate()
    except:
      pass
      output = ""
      print "ignore errors"

    rawEvents = []

    for line in output.splitlines():
      rawEvents.append(RawEvent(line))

    return rawEvents

  def list(self):
    for event in self.get():
      print event