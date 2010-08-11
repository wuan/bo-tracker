#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9"

#include "exception/Base.h"
#include "network/Transfer.h"

namespace blitzortung {
  namespace network {

    Transfer::Transfer(Queue<bo::data::sample::Base>& sampleQueue, const Creds& creds)
      : sampleQueue_(sampleQueue),
      creds_(creds),
      samples_(new data::sample::Base::V()),
      logger_("network.Transfer")
    {
      sleepTime_ = 20;
      eventRateLimit_ = 1.0;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "initalized(sleep: " << sleepTime_ << ", eventRateLimit: " << eventRateLimit_ << ")";
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

    int Transfer::openConnection() {
      struct hostent *hostinfo;
      int sockfd;

      sockfd = socket (AF_INET, SOCK_STREAM, 0);

      if (sockfd == -1) {
	return 0;
      }

      sockaddr_in serv_addr;
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(creds_.getServerport());
      serv_addr.sin_addr.s_addr = inet_addr(creds_.getServername().c_str());

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "openConnection() host: " << creds_.getServername() << " port " << creds_.getServerport();

      if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
	/* host not given by IP but by name */
	hostinfo = gethostbyname (creds_.getServername().c_str());
	if (hostinfo == NULL) {
	  close(sockfd);
	  return 0;
	}
	memcpy((char*) &serv_addr.sin_addr.s_addr, hostinfo->h_addr, hostinfo->h_length);
      }

      if (connect(sockfd, (sockaddr *) &serv_addr, sizeof(sockaddr)) == -1) {
	return 0;
      }

      return sockfd;
    }

    std::string Transfer::sampleToString(const data::sample::Base& sample) {

      std::ostringstream oss;

      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S.%f");
      std::locale oldLocale = oss.imbue(std::locale(std::locale::classic(), timefacet));

      pt::ptime now = pt::second_clock::universal_time();

      pt::time_duration sampleTime = sample.getTime(1);

      pt::ptime sampleDateTime(now.date(), sampleTime);

      // check if the sample is from the day before
      if (sampleTime > now.time_of_day())
	sampleDateTime -= gr::days(1);

      oss << sampleDateTime;
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
	  samples_->push_back(sampleQueue_.pop());
	}

	pt::ptime now = pt::second_clock::universal_time();
	if (now - lastSent >= pt::seconds(sleepTime_)) {

	  if (samples_->size() > 0) {

	    double secondsElapsed = ((now - lastSent).total_milliseconds() / 1000);
	    double eventRate = double(samples_->size()) / secondsElapsed;

	    lastSent = now;

	    if (logger_.isInfoEnabled())
	      logger_.infoStream() << "() sending " << samples_->size() << " samples (rate " << eventRate << " samples/second) at " << now;

	    if (eventRate > eventRateLimit_) {
	      samples_->sort(data::sample::Base::CompareAmplitude());

	      logger_.infoStream() << "() ratelimit " << eventRateLimit_ << " reached, interval seconds: " << secondsElapsed;
	      int sampleLimit = eventRateLimit_ * secondsElapsed;

	      samples_->erase(samples_->begin() + sampleLimit, samples_->end());

	      logger_.infoStream() << "() erasing elements to have " << sampleLimit << " elements (new # of elements: " << samples_->size() << ")";

	      // time sort samples
	      samples_->sort();
	    }

	    // open network connection
	    int sockfd =  openConnection ();

	    // loop through all current samples
	    for (data::sample::Base::VI sample = samples_->begin(); sample != samples_->end(); sample++) {
	      
	      std::string data = sampleToString(*sample);

	      if (logger_.isInfoEnabled())
		logger_.infoStream() << data.substr(0, data.size() -1);

	      if (send (sockfd, data.c_str(), data.size(), 0) == -1) {
		logger_.warnStream() << "() error transmitting data";
		break;
	      }
	    }

	    send (sockfd, "\n", 2, 0);
	    close (sockfd);

	    if (logger_.isDebugEnabled())
		logger_.debugStream() << "connection closed";

	    // delete all samples
	    samples_->clear();
	  }

	}
      }
    }
  }
}
