#include "data/Format.h"
#include "data/WaveformOf.h"
#include "util/Stream.h"

namespace blitzortung {
  namespace data {

    Format::Format(unsigned char numberOfBytes, unsigned char numberOfChannels, unsigned short numberOfSamples) :
      sampleType_(Type::BYTE),
      numberOfChannels_(numberOfChannels),
      numberOfSamples_(numberOfSamples),
      createWaveformWithTimestamp_(0),
      createWaveformFromStream_(0),
      logger_("data.Format")
    {
      updateSampleType(numberOfBytes);
      updateFactoryMethod();
    }

    Format::Format() :
      sampleType_(Type::BYTE),
      numberOfChannels_(0),
      numberOfSamples_(0),
      createWaveformWithTimestamp_(0),
      createWaveformFromStream_(0),
      logger_("data.Format")
    {
    }

    Format::Format(std::iostream& stream) :
      sampleType_(Type::BYTE),
      numberOfChannels_(0),
      numberOfSamples_(0),
      createWaveformWithTimestamp_(0),
      createWaveformFromStream_(0),
      logger_("data.Format")
    {
      fromStream(stream);
    }

    Format::~Format() = default;

    Format& Format::operator=(const Format& other) {
      numberOfChannels_ = other.numberOfChannels_;
      numberOfSamples_ = other.numberOfSamples_;
      sampleType_ = other.sampleType_;
      createWaveformWithTimestamp_ = other.createWaveformWithTimestamp_;
      createWaveformFromStream_ = other.createWaveformFromStream_;
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
      updateFactoryMethod();
    }

    Waveform::AP Format::createEmptyWaveform(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 0, 1>(t0, dt));
    }

    Waveform::AP Format::createEmptyWaveform(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 0, 1>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_1_1(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 1, 1>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_1_1(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 1, 1>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_64_1(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 64, 1>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_64_1(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 64, 1>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_128_1(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 128, 1>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_128_1(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 128, 1>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_256_1(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 256, 1>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_256_1(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 256, 1>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_1_2(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 1, 2>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_1_2(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 1, 2>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_64_2(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 64, 2>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_64_2(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 64, 2>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_128_2(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 128, 2>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_128_2(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 128, 2>(date, stream));
    }

    Waveform::AP Format::createWaveformChar_256_2(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed char, 256, 2>(t0, dt));
    }

    Waveform::AP Format::createWaveformChar_256_2(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed char, 256, 2>(date, stream));
    }

    Waveform::AP Format::createWaveformShort_1_2(const pt::ptime& t0, const pt::time_duration& dt) {
      return Waveform::AP(new WaveformOf<signed short, 1, 2>(t0, dt));
    }

    Waveform::AP Format::createWaveformShort_1_2(const gr::date& date, std::iostream& stream) {
      return Waveform::AP(new WaveformOf<signed short, 1, 2>(date, stream));
    }

    Waveform::AP Format::createWaveform(const pt::ptime& t0, const pt::time_duration& dt) const {
      return (*createWaveformWithTimestamp_)(t0, dt);
    }

    Waveform::AP Format::createWaveform(const gr::date& date, std::iostream& stream) const {
      return (*createWaveformFromStream_)(date, stream);
    }

    void Format::updateFactoryMethod() {
      switch (sampleType_) {

	case Type::BYTE:
	  switch (numberOfChannels_) {
	    case 1:
	      switch (numberOfSamples_) {
		case 0:
		  createWaveformWithTimestamp_ = &Format::createEmptyWaveform;
		  createWaveformFromStream_ = &Format::createEmptyWaveform;
		  break;

		case 1:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_1_1;
		  createWaveformFromStream_ = &Format::createWaveformChar_1_1;
		  break;

		case 64:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_64_1;
		  createWaveformFromStream_ = &Format::createWaveformChar_64_1;
		  break;

		case 128:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_128_1;
		  createWaveformFromStream_ = &Format::createWaveformChar_128_1;
		  break;

		case 256:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_256_1;
		  createWaveformFromStream_ = &Format::createWaveformChar_256_1;
		  break;

		default:
	          throw exception::Base("unhandled number of samples");
	      }
	      break;

	    case 2:
	      switch (numberOfSamples_) {
		case 1:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_1_2;
		  createWaveformFromStream_ = &Format::createWaveformChar_1_2;
		  break;

		case 64:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_64_2;
		  createWaveformFromStream_ = &Format::createWaveformChar_64_2;
		  break;

		case 128:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_128_2;
		  createWaveformFromStream_ = &Format::createWaveformChar_128_2;
		  break;

		case 256:
		  createWaveformWithTimestamp_ = &Format::createWaveformChar_256_2;
		  createWaveformFromStream_ = &Format::createWaveformChar_256_2;
		  break;

		default:
	          throw exception::Base("unhandled number of samples");
	      }
	      break;

	    default:
	      throw exception::Base("unhandled number of channels");
	  }
	  break;

	case Type::SHORT:
	  switch (numberOfChannels_) {
	    case 2:
	      switch (numberOfSamples_) {
		case 1:
		  createWaveformWithTimestamp_ = &Format::createWaveformShort_1_2;
		  createWaveformFromStream_ = &Format::createWaveformShort_1_2;
		  break;

		default:
	          throw exception::Base("unhandled number of samples");
	      }
	      break;

	    default:
	      throw exception::Base("unhandled number of channels");
	  }
	  break;

	default:
	  throw exception::Base("unhandled sample type");
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
