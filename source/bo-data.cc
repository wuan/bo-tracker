/*  

    Copyright (C) 2010-2011  Andreas Würl <blitzortung@tryb.de>

*/

#include "config.h"

#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <json/json.h>

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

inline float sqr(float x) {
  return x*x;
}

inline float sgn(float x)
{
  if (x<0) {
    return -1.0;
  } else {
    if (x>0)
      return 1.0;
    else
      return 0.0;
  }
}

class AbstractOutput {
 
  public:
    typedef std::unique_ptr<AbstractOutput> AP;

    virtual void add(const int index, const bo::data::Event& event) = 0;

    virtual std::string getOutput() const = 0;
};

class JsonOutput : public AbstractOutput {

  protected:
    json_object* jsonArray_;

  public:

    JsonOutput() {
      jsonArray_ = json_object_new_array();
    }

    virtual ~JsonOutput() {
      json_object_put(jsonArray_);
    }

    std::string getOutput() const {
      return std::string(json_object_to_json_string(jsonArray_)) + '\n';
    }

};

class StreamOutput : public AbstractOutput {

  protected:

    std::ostringstream stream_;

    std::string getTimestampString(const bo::data::Event& event) const {
      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S.%f");

      std::ostringstream oss;
      oss.imbue(std::locale(std::locale::classic(), timefacet));
      oss << event.getWaveform().getTime();

      return oss.str();
    }

  public:

   StreamOutput() {
      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S.%f");
      std::locale oldLocale = stream_.imbue(std::locale(std::locale::classic(), timefacet));
   }

   std::string getOutput() const {
     return stream_.str();
   }

};

class DefaultStreamOutput : public StreamOutput {

  public:
    void add(const int index, const bo::data::Event& event) {
      stream_ << event << " " << index << std::endl;
    }
};

class DefaultJsonOutput : public JsonOutput {

  public:
    void add(const int index, const bo::data::Event& event) {
      json_object* jsonArray = event.asJson();
      json_object_array_add(jsonArray, json_object_new_int(index));
      json_object_array_add(jsonArray_, jsonArray);
    }
};

class LongStreamOutput : public StreamOutput {

  private:

    bool normalize_;

  public:

    LongStreamOutput(bool normalize) :
      normalize_(normalize)
    {}

    void add(const int index, const bo::data::Event& event) {
      stream_ << "# " << event << " " << index << std::endl;

      const bo::data::Waveform& waveform = event.getWaveform();

      unsigned int maxIndex = waveform.getMaxIndex();
      float angle = -waveform.getPhase(waveform.getMaxIndexNoClip());
      float cos_angle = cos(angle);
      float sin_angle = sin(angle);

      stream_ << "max index " << maxIndex << " at " << waveform.getTime(maxIndex) << waveform.getFloat(maxIndex, 0) << waveform.getFloat(maxIndex, 1) << std::endl;
      float scaleFactor = 1 << (waveform.getElementSize() * 8 - 1);
      unsigned int numberOfChannels = waveform.getNumberOfChannels();
      unsigned int timeDeltaNanoseconds = waveform.getTimeDelta().total_nanoseconds();

      float lastphase = 0.0;
      int phaseOffset = 0;
      const float phaseCorrectionFactor = 0.5;
      for (unsigned int sample = 0; sample < waveform.getNumberOfSamples(); sample++) {
	stream_ << waveform.getTime(sample) << " " << timeDeltaNanoseconds * sample;
	double sum = 0.0;
	for (unsigned int channel = 0; channel < numberOfChannels; channel++) {
	  double value = waveform.getFloat(sample, channel) / scaleFactor;
	  sum += value * value;
	  stream_ << " " << value;
	}
	if (numberOfChannels > 1) {
	  stream_ << " " << sqrt(sum);

	  if (numberOfChannels == 2) {
	    float phaseval = waveform.getPhase(sample);

	    if (phaseval*lastphase < -sqr(phaseCorrectionFactor*M_PI)) {
	      phaseOffset += int(sgn(lastphase));
	    }
	    lastphase = phaseval;
	    stream_ << " " << phaseval << " " << phaseval + phaseOffset*2.0*M_PI;
	    
	    float x = waveform.getFloat(sample, 0) / scaleFactor;
	    float y = waveform.getFloat(sample, 1) / scaleFactor;
	    stream_ << " " << x * cos_angle - y * sin_angle << " " << x * sin_angle + y * cos_angle;
	  }
	}
	stream_ << std::endl;
      }
      stream_ << std::endl;
    }
};

class LongJsonOutput : public JsonOutput {

  private:

    bool normalize_;

  public:

    LongJsonOutput(bool normalize) :
      normalize_(normalize)
    {}

    void add(const int index, const bo::data::Event& event) {
      json_object* jsonArray = event.asJson();
      json_object_array_add(jsonArray, event.getWaveform().asJson(normalize_));
      json_object_array_add(jsonArray, json_object_new_int(index));
      json_object_array_add(jsonArray_, jsonArray);
    }
};


class TimestampStreamOutput : public StreamOutput {
  public:
    void add(const int index, const bo::data::Event& event) {
      const blitzortung::data::Waveform& waveform = event.getWaveform();
      stream_ << waveform.getTime(waveform.getMaxIndex()) << " " << index << std::endl;
    }
};

class TimestampJsonOutput : public JsonOutput {

  public:
    void add(const int index, const bo::data::Event& event) {
      const blitzortung::data::Waveform& wfm = event.getWaveform();

      json_object* jsonArray = json_object_new_array();

      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S");

      std::ostringstream oss;
      std::locale oldLocale = oss.imbue(std::locale(std::locale::classic(), timefacet));

      oss << wfm.getTime(0);

      json_object_array_add(jsonArray, json_object_new_string(oss.str().c_str()));

      json_object_array_add(jsonArray, json_object_new_int(wfm.getTime(0).time_of_day().fractional_seconds()));

      json_object_array_add(jsonArray, json_object_new_int(index));

      json_object_array_add(jsonArray_, jsonArray);
    }
};

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

int main(int argc, char **argv) {

  std::string file = "";
  std::string mode = "default";
  std::string startTimeString, endTimeString;
  int startIndex, numberOfEvents;
  bool normalize = false;

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
    ("normalize", "reduce multiple channel to single best channel, works only in combinatino with long-data mode")
    ("json,j", "output JSON data")
    ("long-data,l", "output all samples")
    ("event-time", "output eventtime")
    ("debug", "debug mode")
    ;

  // parse command line options
  po::variables_map vm;
  bool showHelp = false;

  try {
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm); 
  } catch (std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    showHelp = true;
  }

  // help output or no 'sql-statement' given
  if (vm.count("help") || showHelp) {
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

    std::cout << start->front().getTimestampAsString() << " " << 0 << std::endl;
    std::cout << end->front().getTimestampAsString() << " " << header.getNumberOfEvents() - 1 << std::endl;

    const bo::data::Format& format = header.getDataFormat();

    std::cout << header.getNumberOfEvents() << " events, ";
    std::cout << format.getNumberOfSamples() << " samples, ";
    std::cout << format.getNumberOfChannels() << " channels, ";
    std::cout << format.getNumberOfBytesPerSample()*8 << " bits";
    std::cout << std::endl;

    return 0;
  }

  if (vm.count("normalize")) {
    normalize = true;
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
      AbstractOutput::AP output;
      if (vm.count("json")) {
	if (vm.count("long-data")) {
	  output = AbstractOutput::AP(new LongJsonOutput(normalize));
	} else if (vm.count("event-time")) {
	  output = AbstractOutput::AP(new TimestampJsonOutput());
	} else {
	  output = AbstractOutput::AP(new DefaultJsonOutput());
	}
      } else {
	if (vm.count("long-data")) {
	  output = AbstractOutput::AP(new LongStreamOutput(normalize));
	} else if (vm.count("event-time")) {
	  output = AbstractOutput::AP(new TimestampStreamOutput());
	} else {
	  output = AbstractOutput::AP(new DefaultStreamOutput());
	}
      }

      int index = events.getStartIndex();
      for (bo::data::Events::CI event = events.begin(); event != events.end(); event++) {
	output->add(index++, *event);
      }

      std::cout << output->getOutput();
  } else {
    std::cerr << "unknown mode '" << mode << "'";
    exit(1);
  }
}
