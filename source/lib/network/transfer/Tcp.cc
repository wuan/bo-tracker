#include "exception/Base.h"
#include "network/transfer/Tcp.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      Tcp::Tcp(Queue<data::sample::Base>& sampleQueue, const Creds& creds, const std::string& outputFile)
	: Base(sampleQueue, creds, outputFile),
	logger_("network.transfer.Tcp")
      {
	sleepTime_ = 20;
	eventRateLimit_ = 1.0;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initalized( sleep: " << sleepTime_ << ", eventRateLimit: " << eventRateLimit_ << ")";
      }

      Tcp::~Tcp() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      int Tcp::openConnection() {
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

      void Tcp::operator()() {

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
		logger_.debugStream() << "() connection closed";

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
}
