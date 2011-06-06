/*  

    Copyright (C) 2010       Andreas Würl <awuerl@gmx.net>

*/

#include <iostream>
#include <string>

#include <boost/program_options.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/density.hpp>

#include "namespaces.h"
#include "data/Events.h"
#include "exception/Base.h"
#include "Logger.h"

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

void printAllSamplesOfEvent(const bo::data::Event& event) {

  pt::time_facet *timefacet = new pt::time_facet();
  timefacet->format("%Y-%m-%d %H:%M:%S.%f");
  std::locale oldLocale = std::cout.imbue(std::locale(std::locale::classic(), timefacet));

  std::cout << "# " << event << std::endl;

  const bo::data::Waveform& waveform = event.getWaveform();

  unsigned int numberOfChannels = waveform.getNumberOfChannels();
  for (unsigned int sample = 0; sample < waveform.getNumberOfSamples(); sample++) {
    std::cout << waveform.getTime(sample);
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

  std::cout.imbue(oldLocale);
}

int main(int argc, char **argv) {

  std::string file = "";
  std::string mode;
  std::string startTimeString, endTimeString;

  bo::Logger logger("");

  // programm arguments/options
  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help,h", "show program help")
    ("input-file,i", po::value<std::string>(&file), "file name")
    ("starttime,s", po::value<std::string>(&startTimeString), "start time in HHMM or HHMMSS format")
    ("endtime,e", po::value<std::string>(&endTimeString), "end time in HHMM or HHMMSS format")
    ("mode", po::value<std::string>(&mode)->default_value("plain"), "data mode( plain, statistics, histogram)")
    ("verbose,v", "verbose mode")
    ("long-data,l", "output all samples")
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

  pt::time_duration startTime(pt::not_a_date_time);
  if (vm.count("starttime")) {
    startTime = parseTime(startTimeString);
  }

  pt::time_duration endTime(pt::not_a_date_time);
  if (vm.count("endtime")) {
    endTime = parseTime(endTimeString, true);
  }

  bo::data::Events events;

  events.readFromFile(file, startTime, endTime);

  void (*eventOperation)(const bo::data::Event&);

  eventOperation = &printEvent;

  if (mode == "statistics") {
    double sum = 0.0;
    for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
      const bo::data::Waveform& waveform = event->getWaveform();
      sum += waveform.getAmplitude(waveform.getMaxIndex());
    }
    double averageAmplitude = sum/events.size();

    sum = 0.0;
    for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
      const bo::data::Waveform& waveform = event->getWaveform();
      double distance = averageAmplitude - waveform.getAmplitude(waveform.getMaxIndex());
      sum += distance * distance;
    }
    double amplitudeVariance = sqrt(sum)/events.size();

    std::cout << events.size() << " " << averageAmplitude << " " << amplitudeVariance << std::endl;
  } else if (mode == "histogram") {
    ac::accumulator_set<double, ac::features<ac::tag::density> > acc(acc::tag::density::num_bins=20);

    for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
      const bo::data::Waveform& waveform = event->getWaveform();
      acc(waveform.getAmplitude(waveform.getMaxIndex()));
    }

    ac::histogram_type hist = density(acc);
    for (int i = 0; i < hist.size(); i++) {
      std::cout << hist[i].first << " " << hist[i].second << std::endl;
    }
  } else if (mode == "default") {
    if (vm.count("long-data"))
      eventOperation = &printAllSamplesOfEvent;

    for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
      (*eventOperation)(*event);
    }
  } else {
    std::cerr << "unknown mode '" << mode << "'";
    exit(1);
  }
}
