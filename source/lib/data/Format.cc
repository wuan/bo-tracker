#include "data/Format.h"
#include "util/Stream.h"

namespace blitzortung {
  namespace data {

    Format::Format(unsigned char numberOfBits, unsigned char numberOfChannels, unsigned short numberOfSamples) :
      numberOfBits_(numberOfBits),
      numberOfChannels_(numberOfChannels),
      numberOfSamples_(numberOfSamples),
      logger_("data.Format")
    {
      updateSizes();
    }

    Format::Format() :
      numberOfBits_(0),
      numberOfChannels_(0),
      numberOfSamples_(0),
      logger_("data.Format")
    {
      updateSizes();
    }

    Format::Format(std::iostream& stream) :
      logger_("data.Format")
    {
      util::Stream::ReadValue(stream, numberOfSamples_);
      util::Stream::ReadValue(stream, numberOfChannels_);
      util::Stream::ReadValue(stream, numberOfBits_);
      updateSizes();
    }

    Format::~Format() {
    }

    unsigned short Format::getNumberOfBitsPerSample() const {
      return numberOfBits_;
    }

    unsigned short Format::getNumberOfChannels() const {
      return numberOfChannels_;
    }

    unsigned short Format::getNumberOfSamples() const {
      return numberOfSamples_;
    }

    void Format::updateSizes() {
      unsigned short bitBytes = numberOfBits_ + 7;
      bitBytes /= 8;
      sampleByteSize_ = bitBytes;

      dataByteSize_ = sampleByteSize_ * numberOfChannels_ * numberOfSamples_;
    }

    unsigned short Format::getBytesPerSample() const {
      return sampleByteSize_;
    }

    unsigned int Format::getDataSize() const {
      return dataByteSize_;
    }

    unsigned int Format::getIndex(unsigned short index, unsigned char channel) const {
      return (index * numberOfChannels_ + channel) * sampleByteSize_;
    }

    void Format::toStream(std::iostream& stream) const {
      util::Stream::WriteValue(stream, numberOfSamples_);
      util::Stream::WriteValue(stream, numberOfChannels_);
      util::Stream::WriteValue(stream, numberOfBits_);
    }

    bool Format::operator==(const Format& other) const {
      return numberOfBits_ == other.numberOfBits_ &&
	numberOfSamples_ == other.numberOfSamples_ &&
	numberOfChannels_ == other.numberOfChannels_;
    }

    bool Format::operator!=(const Format& other) const {
      return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const Format& format) {
      os << "(";
      os << int(format.getNumberOfBitsPerSample()) << " bit, ";
      os << int(format.getNumberOfChannels()) << " ch, ";
      os << format.getNumberOfSamples() << " samples = ";
      os << format.getDataSize() << " bytes)";

      return os;
    }

  }
}