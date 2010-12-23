#include "data/Format.h"

namespace blitzortung {
  namespace data {

    Format::Format(unsigned short numberOfBits, unsigned short numberOfChannels, unsigned int numberOfSamples) :
      numberOfBits_(numberOfBits),
      numberOfChannels_(numberOfChannels),
      numberOfSamples_(numberOfSamples)
    {
    }

    Format::Format() :
      numberOfBits_(0),
      numberOfChannels_(0),
      numberOfSamples_(0)
    {
    }

    Format::~Format() {
    }

    unsigned char Format::getNumberOfBitsPerSample() const {
      return numberOfBits_;
    }

    unsigned char Format::getNumberOfChannels() const {
      return numberOfChannels_;
    }

    unsigned short Format::getNumberOfSamples() const {
      return numberOfSamples_;
    }

    unsigned int Format::getDataSize() const {
      int bitBytes = numberOfBits_ + 7;
      bitBytes /= 8;

      return bitBytes * numberOfChannels_ * numberOfSamples_;
    }

    void Format::fromStream(std::iostream&) {
    }

    void Format::toStream(std::iostream&) const {
    }

    bool Format::operator==(const Format& other) const {
      return numberOfBits_ == other.numberOfBits_ &&
	numberOfSamples_ == other.numberOfSamples_ &&
	numberOfChannels_ == other.numberOfChannels_;
    }

    std::ostream& operator<<(std::ostream& os, const Format& format) {
      os << format.getNumberOfBitsPerSample() << " ";
      os << format.getNumberOfSamples() << " ";
      os << format.getNumberOfChannels();

      return os;
    }

  }
}
