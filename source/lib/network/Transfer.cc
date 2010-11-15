#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9.1"

#include "exception/Base.h"
#include "network/Transfer.h"

namespace blitzortung {
  namespace network {

    Transfer::Transfer(Queue<data::sample::Base>& sampleQueue, const Creds& creds, const std::string& outputFile)
      : sampleQueue_(sampleQueue),
      creds_(creds),
      samples_(new data::sample::Base::V()),
      outputFile_(outputFile),
      logger_("network.Transfer")
    {
      sleepTime_ = 20;
      eventRateLimit_ = 1.0;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "initalized( sleep: " << sleepTime_ << ", eventRateLimit: " << eventRateLimit_ << ")";
    }

    Transfer::~Transfer() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "deleted";
    }

    void Transfer::setSleepTime(const int sleepTime) {
      sleepTime_ = sleepTime;
    }

    void Transfer::setEventRateLimit(const double eventRateLimit) {
      eventRateLimit_ = eventRateLimit;
    }
    
    data::sample::Base::VP Transfer::prepareData(pt::ptime& now, pt::ptime& lastSent) {
      data::sample::Base::VP deletedSamples(new data::sample::Base::V());

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() sample vector contains " << samples_->size() << " elements";

      double secondsElapsed = ((now - lastSent).total_milliseconds() / 1000);
      double eventRate = double(samples_->size()) / secondsElapsed;

      lastSent = now;

      if (logger_.isInfoEnabled())
	logger_.infoStream() << "() sending " << samples_->size() << " samples (rate " << eventRate << " samples/second) at " << now;

      if (eventRate > eventRateLimit_) {
	samples_->sort(data::sample::Base::CompareAmplitude());

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "() ratelimit " << eventRateLimit_ << " reached, interval seconds: " << secondsElapsed;

	int sampleLimit = eventRateLimit_ * secondsElapsed;

	//samples_->transfer(samples_->begin() + sampleLimit, samples_->end());
	deletedSamples->transfer(deletedSamples->end(), samples_->begin(), samples_->end(), *samples_);

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "() erasing elements to have " << sampleLimit << " elements (new # of elements: " << samples_->size() << ")";

	// time sort samples
	samples_->sort();
      }
      
      return deletedSamples;
    }

    void Transfer::sendData() {
      int sock_id;

      sock_id = socket (AF_INET, SOCK_DGRAM, 0);

      if (sock_id == -1) {
	logger_.warnStream() << "could not open socket";
	
	return;
      }

      sockaddr_in serv_addr;
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(creds_.getServerport());
      serv_addr.sin_addr.s_addr = inet_addr(creds_.getServername().c_str());

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "openConnection() host: " << creds_.getServername() << " port " << creds_.getServerport();

      if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
	/* host not given by IP but by name */
	hostent *hostinfo = gethostbyname (creds_.getServername().c_str());
	
	if (hostinfo == NULL) {
	  close(sock_id);
	  logger_.warnStream() << "could not get host information for '" << creds_.getServername() << "'";
	  
	  return;
	}
	memcpy((char*) &serv_addr.sin_addr.s_addr, hostinfo->h_addr, hostinfo->h_length);
      }

      if (logger_.isInfoEnabled())
	logger_.infoStream() << "send data to '" << creds_.getServername() << "' port " << creds_.getServerport();

      // loop through all current samples
      for (data::sample::Base::VI sample = samples_->begin(); sample != samples_->end(); sample++) {
      
	std::string data = sampleToString(*sample);

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << data.substr(0, data.size() -1);

	if (sendto(sock_id, data.c_str(), data.size(), 0, (sockaddr*) &serv_addr, sizeof(sockaddr)) == -1) {
	  logger_.warnStream() << "() error transmitting data";
	  break;
	}
      }
      
      close(sock_id);
      
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() connection closed";
    }

    std::string Transfer::sampleToString(const data::sample::Base& sample) {

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

    void Transfer::saveData() {
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

    void Transfer::operator()() {

      pt::ptime lastSent = pt::second_clock::universal_time();

      while (true) {
	boost::xtime xt;
	boost::xtime_get(&xt, boost::TIME_UTC);
	xt.sec += 1;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "() wait for data";

	sampleQueue_.timed_wait(xt);

	// get new samples from queue until it is empty
	while (! sampleQueue_.empty()) {
	  data::sample::Base::AP sample(sampleQueue_.pop());

	  samples_->push_back(sample);
	}

	pt::ptime now = pt::second_clock::universal_time();

	if (now - lastSent >= pt::seconds(sleepTime_)) {

	  if (samples_->size() > 0) {

	    // prepare data for transmission
	    data::sample::Base::VP deletedSamples = prepareData(now, lastSent);

	    // transmit data
	    sendData();

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() recollect samples " << samples_->size() << " + " << deletedSamples->size();
	    samples_->transfer(samples_->end(), *deletedSamples);

	    samples_->sort();

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() recollected " << samples_->size() << " samples ";

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() output file " << outputFile_;

	    if (outputFile_ != "")
	      saveData();

	    // delete all samples
	    samples_->clear();
	  }

	}
      }
    }
  }
}
