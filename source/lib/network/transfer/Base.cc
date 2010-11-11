#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9.1"

#include "exception/Base.h"
#include "network/transfer/Base.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      Base::Base(Queue<data::sample::Base>& sampleQueue, const Creds& creds, const std::string& outputFile)
	: sampleQueue_(sampleQueue),
	creds_(creds),
	samples_(new data::sample::Base::V()),
	outputFile_(outputFile),
	logger_("network.transfer.Base")
      {
	sleepTime_ = 20;
	eventRateLimit_ = 1.0;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initalized( sleep: " << sleepTime_ << ", eventRateLimit: " << eventRateLimit_ << ")";
      }

      Base::~Base() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      void Base::setSleepTime(const int sleepTime) {
	sleepTime_ = sleepTime;
      }

      void Base::setEventRateLimit(const double eventRateLimit) {
	eventRateLimit_ = eventRateLimit;
      }

      std::string Base::sampleToString(const data::sample::Base& sample) {

	std::ostringstream oss;

	pt::time_facet *timefacet = new pt::time_facet();
	timefacet->format("%Y-%m-%d %H:%M:%S.%f");
	std::locale oldLocale = oss.imbue(std::locale(std::locale::classic(), timefacet));

	oss << sample.getTime(1);
	oss.setf(std::ios::fixed);
	oss.precision(6);

	oss << " " << sample.getAntennaLatitude() << " " << sample.getAntennaLongitude();
	oss << " " << sample.getAntennaAltitude();
	oss << " " << creds_.getUsername() << " " << creds_.getPassword();
	oss << " " << sample.getXAmplitude(1) << " " << sample.getYAmplitude(1);
	oss << " " << sample.getGpsStatus() << " " << VERSION << std::endl;

	// restore original locale
	oss.imbue(oldLocale);

	return oss.str();
      }

      void Base::saveData() {
	data::Samples samples;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "saveData()";

	// move all current samples to
	for (data::sample::Base::VI sample = samples_->begin(); sample != samples_->end();) {

	  if (samples.getDate() != sample->getTime().date() && samples.size() != 0) {
	    samples.appendToFile(outputFile_);
	    samples.clear();
	  }

	  samples.add(samples_->release(sample));
	}

	if (samples.size() > 0) {
	  samples.appendToFile(outputFile_);
	}
      }

    }
  }
}
