/*  

    Copyright (C) 2010       Andreas Würl <awuerl@gmx.net>

*/

#include "config.h"

#include <iostream>
#include <string>

#include <boost/program_options.hpp>
#ifdef HAVE_BOOST_ACCUMULATORS_ACCUMULATORS_HPP
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/density.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#endif

#include "namespaces.h"
#include "data/Events.h"
#include "data/EventsFile.h"
#include "exception/Base.h"
#include "Logger.h"

#ifdef HAVE_BOOST_ACCUMULATORS_ACCUMULATORS_HPP
typedef ac::accumulator_set<double, ac::features<ac::tag::mean, ac::tag::variance> > statistics_accumulator;
typedef ac::accumulator_set<double, ac::features<ac::tag::density> > accumulator;
typedef boost::iterator_range<std::vector<std::pair<double, double> >::iterator > histogram; 
#endif

pt::time_duration parseTime(const std::string& inputString, bool isEnd=false) {
  std::istringstream iss(inputString);
  pt::time_duration endOffset(pt::seconds(0));

  std::string format;
  if (inputString.size() <= 4) {
    format = "%H%M";
    endOffset = pt::minutes(1);
  } else {
    format = "%H%M%S";
    endOffset = pt::seconds(1);
  }

  // create formatting facet and set format for time_duration type
  pt::time_input_facet *facet = new pt::time_input_facet();
  facet->time_duration_format(format.c_str());

  iss.imbue(std::locale(std::locale::classic(), facet));

  pt::time_duration time;
  iss >> time;

  if (isEnd) 
    return time + endOffset;
  else
    return time;
}

void printEvent(const bo::data::Event& event) {
  std::cout << event << std::endl;
}

std::string getTimestampString(const bo::data::Event& event) {
  pt::time_facet *timefacet = new pt::time_facet();
  timefacet->format("%Y-%m-%d %H:%M:%S.%f");

  std::ostringstream oss;

  oss.imbue(std::locale(std::locale::classic(), timefacet));

  oss << event.getWaveform().getTime();

  return oss.str();
}

void printEventTime(const bo::data::Event& event) {
  std::cout << getTimestampString(event) << std::endl;
}

void printAllSamplesOfEvent(const bo::data::Event& event) {

  pt::time_facet *timefacet = new pt::time_facet();
  timefacet->format("%Y-%m-%d %H:%M:%S.%f");
  std::locale oldLocale = std::cout.imbue(std::locale(std::locale::classic(), timefacet));

  std::cout << "# " << event << std::endl;

  const bo::data::Waveform& waveform = event.getWaveform();

  unsigned int numberOfChannels = waveform.getNumberOfChannels();
  unsigned int timeDeltaNanoseconds = waveform.getTimeDelta().total_nanoseconds();
  for (unsigned int sample = 0; sample < waveform.getNumberOfSamples(); sample++) {
    std::cout << waveform.getTime(sample) << " " << timeDeltaNanoseconds * sample;
    double sum = 0.0;
    for (unsigned int channel = 0; channel < numberOfChannels; channel++) {
      double value = waveform.get(sample, channel);
      sum += value * value;
      std::cout << " " << value;
    }
    if (numberOfChannels > 1)
      std::cout << " " << sqrt(sum);
    std::cout << " " << sample;

    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout.imbue(oldLocale);
}

int main(int argc, char **argv) {

  std::string file = "";
  std::string mode = "default";
  std::string startTimeString, endTimeString;
  int startIndex, numberOfEvents;

  bo::Logger logger("");

  // programm arguments/options
  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help,h", "show program help")
    ("input-file,i", po::value<std::string>(&file), "file name")
    ("info", "show file info")
    ("starttime,s", po::value<std::string>(&startTimeString), "start time in HHMM or HHMMSS format")
    ("endtime,e", po::value<std::string>(&endTimeString), "end time in HHMM or HHMMSS format")
    ("start", po::value<int>(&startIndex)->default_value(0), "start index of first event")
    ("number", po::value<int>(&numberOfEvents)->default_value(-1), "number of events")
    #ifdef HAVE_BOOST_ACCUMULATORS_ACCUMULATORS_HPP
    ("mode", po::value<std::string>(&mode)->default_value(mode), "data mode [default, statistics, histogram]")
    #endif
    ("verbose,v", "verbose mode")
    ("long-data,l", "output all samples")
    ("event-time", "output eventtime")
    ("debug", "debug mode")
    ;

  // parse command line options
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm); 

  // help output or no 'sql-statement' given
  if (vm.count("help")) {
    std::cout << argv[0] << " [options]" << std::endl << std::endl;
    std::cout << desc << std::endl;
    std::cout << std::endl;

    return 1;
  }

  if (! vm.count("input-file")) {
    std::cerr << "'input-file' missing\n";
    return 5;
  }

  // logging setup

  logger.setPriority(log4cpp::Priority::NOTICE);

  if (vm.count("verbose")) {
    logger.setPriority(log4cpp::Priority::INFO);
  }

  if (vm.count("debug")) {
    logger.setPriority(log4cpp::Priority::DEBUG);
  }

  if (vm.count("info")) {
    bo::data::EventsFile eventsFile(file);

    const bo::data::EventsHeader& header = eventsFile.getHeader();

    bo::data::Events::AP start(eventsFile.read(0,1));
    bo::data::Events::AP end(eventsFile.read(-1,1));

    std::cout << getTimestampString(start->front()) << " " << 0 << std::endl;
    std::cout << getTimestampString(end->front()) << " " << header.getNumberOfEvents() - 1 << std::endl;

    const bo::data::Format& format = header.getDataFormat();

    std::cout << header.getNumberOfEvents() << " events, ";
    std::cout << format.getNumberOfSamples() << " samples, ";
    std::cout << format.getNumberOfChannels() << " channels, ";
    std::cout << format.getNumberOfBitsPerSample() << " bits";
    std::cout << std::endl;

    return 0;
  }

  bo::data::Events events;

  if (vm.count("starttime") || vm.count("endtime")) {
    pt::time_duration startTime(pt::not_a_date_time);
    if (vm.count("starttime")) {
      startTime = parseTime(startTimeString);
    }

    pt::time_duration endTime(pt::not_a_date_time);
    if (vm.count("endtime")) {
      endTime = parseTime(endTimeString, true);
    }

    events.readFromFile(file, startTime, endTime);
  } else {
    events.readFromFile(file, startIndex, numberOfEvents);
  }

  void (*eventOperation)(const bo::data::Event&);

  eventOperation = &printEvent;

  #ifdef HAVE_BOOST_ACCUMULATORS_ACCUMULATORS_HPP
  if (mode == "statistics") {
    statistics_accumulator acc;

    for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
      const bo::data::Waveform& waveform = event->getWaveform();
      acc(waveform.getAmplitude(waveform.getMaxIndex()));
    }

    std::cout << events.size() << " " << ac::mean(acc) << " " << ac::variance(acc) << std::endl;
  } else if (mode == "histogram") {
    accumulator acc(ac::tag::density::num_bins = 20, ac::tag::density::cache_size = events.size());

    for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
      const bo::data::Waveform& waveform = event->getWaveform();
      acc(waveform.getAmplitude(waveform.getMaxIndex()));
    }

    histogram hist = ac::density(acc);
    for (int i = 0; i < hist.size(); i++) {
      std::cout << hist[i].first << " " << hist[i].second * events.size() << std::endl;
    }
  } else 
  #endif
    if (mode == "default") {
    if (vm.count("long-data"))
      eventOperation = &printAllSamplesOfEvent;
    else if (vm.count("event-time"))
      eventOperation = &printEventTime;

    for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
      (*eventOperation)(*event);
    }
  } else {
    std::cerr << "unknown mode '" << mode << "'";
    exit(1);
  }
}
