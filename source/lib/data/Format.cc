#include "data/Format.h"
#include "data/WaveformOf.h"
#include "util/Stream.h"

namespace blitzortung {
  namespace data {

    Format::Format(unsigned char numberOfBytes, unsigned char numberOfChannels, unsigned short numberOfSamples) :
      sampleType_(Type::BYTE),
      numberOfChannels_(numberOfChannels),
      numberOfSamples_(numberOfSamples),
      logger_("data.Format")
    {
      updateSampleType(numberOfBytes);
    }

    Format::Format() :
      sampleType_(Type::BYTE),
      numberOfChannels_(0),
      numberOfSamples_(0),
      logger_("data.Format")
    {
    }

    Format::Format(std::iostream& stream) :
      sampleType_(Type::BYTE),
      numberOfChannels_(0),
      numberOfSamples_(0),
      logger_("data.Format")
    {
      fromStream(stream);
    }

    Format::~Format() = default;

    Format& Format::operator=(const Format& other) {
      numberOfChannels_ = other.numberOfChannels_;
      numberOfSamples_ = other.numberOfSamples_;
      sampleType_ = other.sampleType_;
      return *this;
    }

    unsigned char Format::getNumberOfChannels() const {
      return numberOfChannels_;
    }

    unsigned short Format::getNumberOfSamples() const {
      return numberOfSamples_;
    }

    void Format::updateSampleType(unsigned char numberOfBytesPerSample) {      

      if (numberOfBytesPerSample == 1) {
	sampleType_ = Type::BYTE;
      } else if (numberOfBytesPerSample == 2) {
	sampleType_ = Type::SHORT;
      } else if (numberOfBytesPerSample == 4) {
	sampleType_ = Type::INT;
      } else {
	// handle older format with number of bits per sample
	if (numberOfBytesPerSample > 8) {
	  sampleType_ = Type::SHORT;
	} else {
	  sampleType_ = Type::BYTE;
	}
      }
    }

    Format::Type Format::getDataType() const {
      return sampleType_;
    }

    unsigned char Format::getNumberOfBytesPerSample() const {
      return (unsigned char)(sampleType_);
    }

    unsigned int Format::getDataSize() const {
      return getNumberOfBytesPerSample() * numberOfChannels_ * numberOfSamples_;
    }

    bool Format::isValid() const {
      return getDataSize() > 0;
    }

    void Format::toStream(std::iostream& stream) const {
      util::Stream::WriteValue(stream, numberOfSamples_);
      util::Stream::WriteValue(stream, numberOfChannels_);
      util::Stream::WriteValue(stream, (unsigned char)sampleType_);
    }

    void Format::fromStream(std::iostream& stream) {
      util::Stream::ReadValue(stream, numberOfSamples_);
      util::Stream::ReadValue(stream, numberOfChannels_);
      {
	unsigned char numberOfBytesPerSample;
	util::Stream::ReadValue(stream, numberOfBytesPerSample);
	updateSampleType(numberOfBytesPerSample);
      }
    }

    Waveform::AP Format::createWaveform(const pt::ptime& t0, const pt::time_duration& dt) const {
      switch (sampleType_) {

	case Type::BYTE:
	  return Waveform::AP(new WaveformOf<signed char>(numberOfChannels_, numberOfSamples_, t0, dt));

	case Type::SHORT:
	  return Waveform::AP(new WaveformOf<signed short>(numberOfChannels_, numberOfSamples_, t0, dt));

	case Type::INT:
	  return Waveform::AP(new WaveformOf<signed int>(numberOfChannels_, numberOfSamples_, t0, dt));

	default:
	  throw new exception::Base("unknown sample type");
      }
    }

    Waveform::AP Format::createWaveformFromStream(const gr::date& date, std::iostream& stream) const {
      switch (sampleType_) {

	case Type::BYTE:
	  return Waveform::AP(new WaveformOf<signed char>(numberOfChannels_, numberOfSamples_, date, stream));

	case Type::SHORT:
	  return Waveform::AP(new WaveformOf<signed short>(numberOfChannels_, numberOfSamples_, date, stream));

	case Type::INT:
	  return Waveform::AP(new WaveformOf<signed int>(numberOfChannels_, numberOfSamples_, date, stream));

	default:
	  throw new exception::Base("unknown sample type");
      }
    }


    bool Format::operator==(const Format& other) const {
      return sampleType_ == other.sampleType_ &&
	numberOfSamples_ == other.numberOfSamples_ &&
	numberOfChannels_ == other.numberOfChannels_;
    }

    bool Format::operator!=(const Format& other) const {
      return !(*this == other);
    }

    std::ostream& operator<<(std::ostream& os, const Format& format) {
      os << "(";
      os << int(format.getNumberOfBytesPerSample()) << " byte(s), ";
      os << int(format.getNumberOfChannels()) << " ch, ";
      os << format.getNumberOfSamples() << " sample(s) = ";
      os << format.getDataSize() << " bytes)";

      return os;
    }

  }
}
