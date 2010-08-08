#include <arpa/inet.h>
#include <netdb.h>

#include "network/Base.h"
#include "exception/Base.h"

#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9"
#define STRIKES_SERVER_IP       "81.169.143.239"
#define STRIKE_SERVER_PORT      8308

namespace blitzortung {
  namespace network {


    class NetworkTransfer {
      private:
	int sleepTime_;
	Queue<data::sample::Base*>& sampleQueue_;
	const Creds& creds_;
        data::sample::Base::VP samples_;
	double eventRateLimit_;

      public:
	NetworkTransfer(Queue<bo::data::sample::Base*>& sampleQueue, const Creds& creds)
	  : sampleQueue_(sampleQueue),
	  creds_(creds),
	  samples_(new data::sample::Base::V())
	{
	  sleepTime_ = 20;
	  eventRateLimit_ = 1.0;
	}

	virtual ~NetworkTransfer() {
	}

	//! set sleep time between data transfers in seconds
	void setSleepTime(const int sleepTime) {
	  sleepTime_ = sleepTime;
	}

	//! set limit of average number of events per minute transmitted
	void setEventRateLimit(const double eventRateLimit) {
	  eventRateLimit_ = eventRateLimit;
	}

	//! initialize network connection to server
	int open_connection ()
	{
	  struct hostent *hostinfo;
	  int sockfd;
	  char buf [1000];

	  sockfd = socket (AF_INET, SOCK_STREAM, 0);

	  if (sockfd == -1) {
	    return 0;
	  }

	  sockaddr_in serv_addr;
	  serv_addr.sin_family = AF_INET;
	  serv_addr.sin_port = htons(creds_.getServerport());
	  serv_addr.sin_addr.s_addr = inet_addr(creds_.getServername().c_str());

	  if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
	    /* host not given by IP but by name */
	    hostinfo= gethostbyname (creds_.getServername().c_str());
	    if (hostinfo == NULL) {
	      close(sockfd);
	      return 0;
	    }
	    memcpy((char*) &serv_addr.sin_addr.s_addr, hostinfo->h_addr, hostinfo->h_length);
	  }

	  if (connect(sockfd, (sockaddr *) &serv_addr, sizeof(sockaddr)) == -1) {
	    return 0;
	  }

	  if (recv (sockfd, buf, 1000, 0) == -1) {
	    return 0;
	  }
	  //std::cout << "received: " << buf << std::endl;

	  return sockfd;
	}

	//! get string to be transmitted for every sample
	std::string sampleToString(const data::sample::Base& sample) {

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
	  oss << " " << creds_.getUsername() << " " << creds_.getPassword();
	  oss << " " << sample.getXAmplitude(1) << " " << sample.getYAmplitude(1);
	  oss << " " << sample.getGpsStatus() << " " << VERSION << std::endl;

	  // restore original locale
	  oss.imbue(oldLocale);

	  return oss.str();
	}

	//! network transfer thread: wait for new samples and send them from time to time
	void operator ()() {

	  pt::ptime lastSent = pt::second_clock::universal_time();

	  while (true) {
	    boost::xtime xt;
	    boost::xtime_get(&xt, boost::TIME_UTC);
	    xt.sec += 1;

	    sampleQueue_.timed_wait(xt);

	    // get new samples from queue until it is empty
	    while (! sampleQueue_.empty()) {
	      samples_->push_back(sampleQueue_.front());
	      sampleQueue_.pop();
	    }

	    pt::ptime now = pt::second_clock::universal_time();
	    if (now - lastSent >= pt::seconds(sleepTime_)) {


	      if (samples_->size() > 0) {

		double secondsElapsed = ((now - lastSent).total_milliseconds() / 1000);
		double eventRate = double(samples_->size()) / secondsElapsed;

		lastSent = now;

	        //std::cout << "# sending " << samples_->size() << " (rate " << eventRate << " samples/second) at " << now << "\n";

		if (eventRate > eventRateLimit_)
		{
		  samples_->sort(data::sample::Base::CompareAmplitude());

		  //std::cout << "ratelimit: " << eventRateLimit_ << " seconds " << secondsElapsed << std::endl;
		  int sampleLimit = eventRateLimit_ * secondsElapsed;

		  samples_->erase(samples_->begin() + sampleLimit, samples_->end());

		  //std::cout << "*** amplitude sorted after erasing elements to have " << sampleLimit << " elements\n";

		  // time sort samples
		  samples_->sort();
		}

		// open network connection
		int sockfd =  open_connection ();

		// loop through all current samples
		for (data::sample::Base::VI sample = samples_->begin(); sample != samples_->end(); sample++) {
		  std::string data = sampleToString(*sample);

		  //std::cout << data;

		  const char* buf = data.c_str();
		  if (send (sockfd, buf, strlen(buf), 0) == -1) {
		    throw exception::Base("network::Base::TransferThread() error sending data");
		  }
		}

		send (sockfd, "\n\n", 2, 0);
		close (sockfd);

		// delete all samples
		samples_->clear();
	      }

	    }
	  }
	}
    };

    Base::Base(const Creds& creds, const int sleepTime, const double eventRateLimit)
    {
      NetworkTransfer networkTransfer(sampleQueue_, creds);

      networkTransfer.setSleepTime(sleepTime);
      networkTransfer.setEventRateLimit(eventRateLimit);

      boost::thread thread(networkTransfer);
    }

    Base::~Base() {
    }

    void Base::put(std::auto_ptr<data::sample::Base> sample) {
      sampleQueue_.push(sample.release());
    }

  }
}

