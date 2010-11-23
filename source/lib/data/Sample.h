#ifndef BLITZORTUNG_DATA_SAMPLE_H_
#define BLITZORTUNG_DATA_SAMPLE_H_

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "namespaces.h"
#include "data/Waveform.h"
#include "data/GpsInfo.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for gps information
    class Sample : boost::noncopyable {

      public:

	typedef boost::ptr_vector<Sample> V;
	typedef V::iterator VI;
	typedef V::const_iterator CVI;
	typedef boost::shared_ptr<V> VP;
	typedef std::auto_ptr<Sample> AP;

	//! type definition for waveform used in samples
	typedef data::Waveform<short> Waveform;

      private:

	//! recorded waveform
	Waveform::AP waveform_;

	//! corresponding gps information
	GpsInfo::AP gpsInfo_;


      public:

        //! create a Sample object
	/*!
	\param gps const reference to gps hardware
	*/
	Sample(Waveform::AP, GpsInfo::AP);

	//! destroy a waveform object
	virtual ~Sample();

	//! get reference to waveform
	const Waveform& getWaveform() const;

	//! release waveform from sample
	Waveform::AP releaseWaveform();

	//! get reference to gps information
	const GpsInfo& getGpsInfo() const;

	//! release gps information from sample
	GpsInfo::AP releaseGpsInfo();

	//! comparison operator <
	bool operator<(const Sample &) const;

	//! class to compare timestamp of Strokes
	struct CompareAmplitude : public std::binary_function<Sample, Sample, bool> {
	  bool operator()(const first_argument_type& x, const second_argument_type& y) const;
	};

    };

  }
}
#endif
