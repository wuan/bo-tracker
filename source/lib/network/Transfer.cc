#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9.1"

#include "exception/Base.h"
#include "network/Transfer.h"

namespace blitzortung {
  namespace network {

    Transfer::Transfer(Queue<data::Sample>& sampleQueue, const Creds& creds, output::Base& output)
      : sampleQueue_(sampleQueue),
      creds_(creds),
      samples_(new data::Sample::V()),
      output_(output),
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
    
    data::Sample::VP Transfer::prepareData(pt::ptime& now, pt::ptime& lastSent) {
      data::Sample::VP deletedSamples(new data::Sample::V());

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() sample vector contains " << samples_->size() << " elements";

      double secondsElapsed = ((now - lastSent).total_milliseconds() / 1000);
      double eventRate = double(samples_->size()) / secondsElapsed;

      lastSent = now;

      if (logger_.isInfoEnabled())
	logger_.infoStream() << "() sending " << samples_->size() << " samples (rate " << eventRate << " samples/second) at " << now;

      if (eventRate > eventRateLimit_) {
	samples_->sort(data::Sample::CompareAmplitude());

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
      for (data::Sample::VI sample = samples_->begin(); sample != samples_->end(); sample++) {
      
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

    std::string Transfer::sampleToString(const data::Sample& sample) {
      const data::Sample::Waveform& wfm = sample.getWaveform();
      const data::GpsInfo& gpsInfo = sample.getGpsInfo();

      std::ostringstream oss;

      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S.%f");
      std::locale oldLocale = oss.imbue(std::locale(std::locale::classic(), timefacet));

      oss << wfm.getTime();
      oss.setf(std::ios::fixed);
      oss.precision(6);

      oss << " " << gpsInfo.getLatitude() << " " << gpsInfo.getLongitude();
      oss << " " << gpsInfo.getAltitude();
      oss << " " << creds_.getUsername() << " " << creds_.getPassword();
      oss << " " << wfm.getMaxX() << " " << wfm.getMaxY();
      oss << " " << gpsInfo.getStatus() << " " << VERSION << std::endl;

      // restore original locale
      oss.imbue(oldLocale);

      return oss.str();
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
	  data::Sample::AP sample(sampleQueue_.pop());

	  samples_->push_back(sample);
	}

	pt::ptime now = pt::second_clock::universal_time();

	if (now - lastSent >= pt::seconds(sleepTime_)) {

	  if (samples_->size() > 0) {

	    // prepare data for transmission
	    data::Sample::VP deletedSamples = prepareData(now, lastSent);

	    // transmit data
	    sendData();

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() recollect samples " << samples_->size() << " + " << deletedSamples->size();
	    samples_->transfer(samples_->end(), *deletedSamples);

	    samples_->sort();

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() recollected " << samples_->size() << " samples ";

	    output_.output(samples_);

	    // delete all samples
	    samples_->clear();
	  }

	}
      }
    }
  }
}
