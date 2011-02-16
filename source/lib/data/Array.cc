#include "data/Array.h"

namespace blitzortung {
  namespace data {

    Array::Array(const Format::CP& format) :
      format_(format)
    {
      data_ = new char[format_->getDataSize()];
    }

    Array::~Array() {
      if (data_ != 0)
	delete (char*)data_;
    }

    unsigned short Array::getNumberOfBitsPerSample() const {
      return format_->getNumberOfBitsPerSample();
    }

    unsigned int Array::getNumberOfSamples() const {
      return format_->getNumberOfSamples();
    }

    unsigned short Array::getNumberOfChannels() const {
      return format_->getNumberOfChannels();
    }

    void Array::set(int value, unsigned int sample, unsigned short channel) {
      unsigned int index = format_->getIndex(sample, channel);
      if (index > format_->getDataSize() - 1)
	throw exception::Base("Array::set() index out of range");

      switch(format_->getDataType()) {
	case Format::BYTE:
	  ((char*)(data_))[index] = (char)(value);
	  break;

	case Format::SHORT:
	  ((short*)(data_))[index] = (short)(value);
	  break;

	case Format::INT:
	  ((int*)(data_))[index] = value;
	  break;

	default:
	  throw exception::Base("Array::set() unhandled format data type");
      }
    }

    float Array::get(unsigned int sample, unsigned short channel) const {
      float divider = 1 << ( format_->getNumberOfBitsPerSample() - 1);

      switch(format_->getDataType()) {
	case Format::BYTE:
	  return ((char*)(data_))[format_->getIndex(sample, channel)]/divider;

	case Format::SHORT:
	  return ((short*)(data_))[format_->getIndex(sample, channel)]/divider;

	case Format::INT:
	  return ((int*)(data_))[format_->getIndex(sample, channel)]/divider;

	default:
	  throw exception::Base("Array::get() unhandled format data type");
      }
    }

    void Array::toStream(std::iostream& stream) const {
      stream.write((char*)data_, format_->getDataSize());
    }

    void Array::fromStream(std::iostream& stream) {
      stream.read((char*)data_, format_->getDataSize());
    }

    const data::Format::CP& Array::getFormat() const {
      return format_;
    }

    std::ostream& operator<<(std::ostream& os, const Array& array) {
      return os << array.getFormat();
    }

  }
}
