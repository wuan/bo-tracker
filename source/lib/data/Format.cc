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
      updateDataType();
    }

    Format::Format() :
      numberOfBits_(0),
      numberOfChannels_(0),
      numberOfSamples_(0),
      logger_("data.Format")
    {
      updateDataType();
    }

    Format::Format(std::iostream& stream) :
      logger_("data.Format")
    {
      fromStream(stream);
    }

    Format::~Format() = default;

    Format& Format::operator=(const Format& other) {
      numberOfBits_ = other.numberOfBits_;
      numberOfChannels_ = other.numberOfChannels_;
      numberOfSamples_ = other.numberOfSamples_;
      sampleType_ = other.sampleType_;
      return *this;
    }

    unsigned short Format::getNumberOfBitsPerSample() const {
      return numberOfBits_;
    }

    int Format::getSampleZeroOffset() const {
      return -(1 << (getNumberOfBitsPerSample() - 1));
    }

    unsigned short Format::getNumberOfChannels() const {
      return numberOfChannels_;
    }

    unsigned short Format::getNumberOfSamples() const {
      return numberOfSamples_;
    }

    void Format::updateDataType() {      
      if (numberOfBits_ <= 8) {
	sampleType_ = Type::BYTE;
      } else if (numberOfBits_ <= 16) {
	sampleType_ = Type::SHORT;
      } else {
	sampleType_ = Type::INT;
      }
    }

    Format::Type Format::getDataType() const {
      return sampleType_;
    }

    unsigned short Format::getNumberOfBytesPerSample() const {
      return (unsigned short)(sampleType_);
    }

    unsigned int Format::getDataSize() const {
      return getNumberOfBytesPerSample() * numberOfChannels_ * numberOfSamples_;
    }

    unsigned int Format::getIndex(unsigned short index, unsigned char channel) const {
      return index * numberOfChannels_ + channel;
    }

    bool Format::isValid() const {
      return getDataSize() > 0;
    }

    void Format::toStream(std::iostream& stream) const {
      util::Stream::WriteValue(stream, numberOfSamples_);
      util::Stream::WriteValue(stream, numberOfChannels_);
      util::Stream::WriteValue(stream, numberOfBits_);
    }

    void Format::fromStream(std::iostream& stream) {
      util::Stream::ReadValue(stream, numberOfSamples_);
      util::Stream::ReadValue(stream, numberOfChannels_);
      util::Stream::ReadValue(stream, numberOfBits_);

      updateDataType();
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
