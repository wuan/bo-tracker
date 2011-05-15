# -*- coding: utf8 -*-

'''

@author Andreas Würl

'''

import datetime

import geom
import files

class Time(object):

  timeformat = '%Y-%m-%d %H:%M:%S'

  def __init__(self, date, time):
    nanoseconds = int(time[-9:])
    time = datetime.datetime.strptime(date + ' ' + time[:-10], Time.timeformat)
    time = time.replace(microsecond=int(round(nanoseconds / 1000.0)), tzinfo=pytz.UTC)
    self.time = time

  def difference(self, other):
    return self.time - other.time

  def getTime(self):
    return self.time

class TimeRange(object):

  def __init__(self, endTime, deltaTime=datetime.timedelta(hours=2)):
    self.endTime = endTime
    self.startTime = endTime - deltaTime

  def __str__(self):
    return "['" + str(self.startTime) + "':'" + str(self.endTime) + "']"

  def getStartTime(self):
    return self.startTime

  def getEndTime(self):
    return self.endTime

  def getEndMinute(self):
    return self.getEndTime() - datetime.timedelta(minutes=1)

  def contains(self, time):
    return time >= self.getStartTime() and time <= self.getEndTime()

class TimeInterval(TimeRange):

  def __init__(self, endTime, deltaTime=datetime.timedelta(hours=1)):
    self.deltaTime = deltaTime
    TimeRange.__init__(self, self.roundTime(endTime), deltaTime)

  def __str__(self):
    return "['" + str(self.startTime) + "':'" + str(self.endTime) + "'," + str(self.deltaTime) + "]"

  def totalSeconds(self, time):
    ' return the total seconds of the given time or datetime (relative to midnight) '

    if isinstance(time, datetime.datetime):
      return time.hour * 3600 + time.minute * 60 + time.second
    elif isinstance(time, datetime.timedelta):
      return time.seconds + time.days * 24 * 3600
    else:
      raise Exception("unhandled type"+type(time))

  def roundTime(self, time):
    deltaSeconds = self.totalSeconds(self.deltaTime)

    seconds =  self.totalSeconds(time)
    seconds /= deltaSeconds
    seconds *= deltaSeconds

    if isinstance(time, datetime.datetime):
      return time.replace(hour = seconds/3600, minute= seconds / 60 % 60, second= seconds % 60, microsecond=0)
    else:
      return datetime.timedelta(seconds=seconds)

  def hasNext(self):
    return False

  def next(self):
    raise Exception(' no next interval ')

  def getCenterTime(self):
    return self.startTime + self.deltaTime / 2

class TimeIntervals(TimeInterval):

  def __init__(self, endTime, deltaTime=datetime.timedelta(minutes=15), totalDuration=datetime.timedelta(days=1)):
    TimeInterval.__init__(self, endTime, deltaTime)

    self.totalDuration = self.roundTime(totalDuration)

    self.startTime = self.endTime - self.totalDuration

  def hasNext(self):
    return self.startTime + self.deltaTime < self.endTime

  def next(self):
    if self.hasNext():
      self.startTime += self.deltaTime
      return self.startTime
    else:
      raise Exception('no more time intervals')

  def getEndTime(self):
    return self.startTime + self.deltaTime

class Event(geom.Point, Time):

  def __init__(self, x, y, date, time):
    Point.__init__(self, x, y)
    Time.__init__(self, date, time)

class RawEvent(Event):
#2011-02-20 15:16:26.723987041 11.5436 48.1355 521 8 3125 -0.12 0.20 14
  def __init__(self, data = None):
    if data != None:
      ' Construct rawevent from blitzortung text format data line '
      fields = data.split(' ')
      if len(fields) >= 8:
        position_x = float(fields[2])
        position_y = float(fields[3])
        Event.__init__(self, position_x, position_y, fields[0], fields[1])
        self.time += datetime.timedelta(seconds=1)
        self.height = int(fields[4])
        self.numberOfSatellites = int(fields[5])
        self.samplePeriod = int(fields[6])
        self.amplitudeX = float(fields[7])
        self.amplitudeY = float(fields[8])
      else:
        raise Error("not enough data fields for raw event data '%s'" %(data))

  def __str__(self):
    return "%s.%06d %.4f %.4f %d %d %d %.2f %.2f" %(self.time.strftime(Time.timeformat), self.time.microsecond, self.x, self.y, self.height, self.numberOfSatellites, self.samplePeriod, self.amplitudeX, self.amplitudeY)

  def getXAmplitude(self):
    return self.amplitudeX

  def getYAmplitude(self):
    return self.amplitudeY



class Stroke(Event):
  '''
  classdocs
  '''

  def __init__(self, data = None):
    if data != None:
      ' Construct stroke from blitzortung text format data line '
      fields = data.split(' ')
      if len(fields) >= 8:
        position_x = float(fields[3])
        position_y = float(fields[2])
        Event.__init__(self, position_x, position_y, fields[0], fields[1])

        self.amplitude = float(fields[4][:-2])
        self.typeVal = int(fields[5])
        self.error2d = int(fields[6][:-1])
        if self.error2d < 0:
          self.error2d = 0
        self.stationcount = int(fields[7])
        self.participants = []
        if (len(fields) >=9):
          for index in range(8,len(fields)):
            self.participants.append(fields[index])
      else:
        raise Error("not enough data fields from stroke data line '%s'" %(data))
    self.height = 0.0

  def getPosition(self):
    return self.position

  def setPosition(self, position):
    self.position = position

  def getHeight(self):
    return self.height

  def setHeight(self, height):
    self.height = height

  def getAmplitude(self):
    return self.amplitude

  def setAmplitude(self, amplitude):
    self.amplitude = amplitude

  def getType(self):
    return self.typeVal

  def setType(self, typeVal):
    self.typeVal = typeVal

  def getError2D(self):
    return self.error2d

  def setError2D(self, error2d):
    self.error2d = error2d

  def getStationCount(self):
    return self.stationcount

  def setStationCount(self, stationcount):
    self.stationcount = stationcount

  def hasParticipant(self, participant):
    return self.participants.count(participant) > 0

  def __str__(self):
    return str(self.time) + ' ' + str(self.x) + ' ' + str(self.y) + ' ' + str(self.height) + ' ' + str(self.amplitude) + ' ' + str(self.typeVal) + ' ' + str(self.error2d) + ' ' + str(self.stationcount)


class Histogram(files.Data):

  def __init__(self, fileNames, time):
    files.Data.__init__(self, fileNames, time)

    while True:

      # count the number of samples
      counter = 0
      ampsum = 0.0
      for rawEvent in self.get():
        xamp = rawEvent.getXAmplitude()
        yamp = rawEvent.getYAmplitude()
        ampsum += math.sqrt(xamp * xamp + yamp * yamp)
        counter += 1

      time = self.time.getCenterTime()

      if counter > 0:
        avgamp = ampsum/counter
      else:
        avgamp = '?'
      print time, counter, avgamp

      if not self.time.hasNext():
        break

      self.time.next()

class StrokesUrl:

  def __init__(self, baseurl):
    self.url = baseurl

  def add(self, name, value):
    self.url += '&' + str(name).strip() + '=' + str(value).strip()

  def readData(self):
    urlconnection = urllib.urlopen(self.url)
    data = urlconnection.read().strip()
    urlconnection.close()
    return data

  def get(self, timeInterval=None):
    strokes = []
    for line in self.readData().split('\n'):
      stroke = Stroke(line)
      if timeInterval==None or timeInterval.contains(stroke.getTime()):
        strokes.append(stroke)
    return strokes

class Strokes(StrokesUrl):

  def __init__(self, config):
    StrokesUrl.__init__(self, 'http://'+config.get('username')+':'+config.get('password')+'@blitzortung.tmt.de/Data/Protected/strikes.txt')

class ParticipantStrokes(StrokesUrl):

  def __init__(self, config):
    StrokesUrl.__init__(self, 'http://'+config.get('username')+':'+config.get('password')+'@blitzortung.tmt.de/Data/Protected/participants.txt')
