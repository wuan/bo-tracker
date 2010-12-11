#ifndef BLITZORTUNG_DATA_FORMAT_H_
#define BLITZORTUNG_DATA_FORMAT_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    class Format : boost::noncopyable {

      private:

	unsigned short samples_;

	unsigned char channels_;

	unsigned char bits_;

      public:

        //! create a waveform format object
	/*!
	\param bits number of bits per sample
	\param channels number of channels per measurement
	\param numberOfSamples number of measurements per waveform
	*/
	Format(unsigned short bits, unsigned short channels, unsigned int numberOfSamples);

	//! delete waveform object
	virtual ~Format();

	//! returns the number of bits
	unsigned short getNumberOfBitsPerSample() const;

	//! returns the number of channels per measurement
	unsigned short getNumberOfChannels() const;

	//! returns the number of measurements per waveform
	unsigned int getNumberOfSamples() const;

    };

    std::ostream& operator <<(std::ostream& os, const bo::data::Format &format) {
      os << format.getNumberOfBitsPerSample() << " ";
      os << format.getNumberOfSamples() << " ";
      os << format.getNumberOfChannels();
      return os;
    }

  }
}
#endif
