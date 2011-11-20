#include "data/Array.h"

namespace blitzortung {
  namespace data {

    Array::Array(const Format& format) :
      format_(format),
      data_(new  char[format_.getDataSize()])
    {
    }

    Array::Array(Array&& array, const Format& format) :
      format_(format),
      data_(std::move(array.data_))
    {
    }

    unsigned short Array::getNumberOfBitsPerSample() const {
      return format_.getNumberOfBitsPerSample();
    }

    unsigned int Array::getNumberOfSamples() const {
      return format_.getNumberOfSamples();
    }

    unsigned short Array::getNumberOfChannels() const {
      return format_.getNumberOfChannels();
    }

    void Array::set(int value, unsigned int sample, unsigned short channel) {
      unsigned int index = format_.getIndex(sample, channel);
      if (index > format_.getDataSize() - 1)
	throw exception::Base("Array::set() index out of range");

      switch(format_.getDataType()) {
	case Format::Type::BYTE:
	  ((char*)(data_.get()))[index] = (char)(value);
	  break;

	case Format::Type::SHORT:
	  ((short*)(data_.get()))[index] = (short)(value);
	  break;

	case Format::Type::INT:
	  ((int*)(data_.get()))[index] = value;
	  break;

	default:
	  throw exception::Base("Array::set() unhandled format data type");
      }
    }

    float Array::get(unsigned int sample, unsigned short channel) const {
      float divider = 1 << ( format_.getNumberOfBitsPerSample() - 1);

      switch(format_.getDataType()) {
	case Format::Type::BYTE:
	  return ((char*)(data_.get()))[format_.getIndex(sample, channel)]/divider;

	case Format::Type::SHORT:
	  return ((short*)(data_.get()))[format_.getIndex(sample, channel)]/divider;

	case Format::Type::INT:
	  return ((int*)(data_.get()))[format_.getIndex(sample, channel)]/divider;

	default:
	  throw exception::Base("Array::get() unhandled format data type");
      }
    }

    void Array::toStream(std::iostream& stream) const {
      stream.write((char*)data_.get(), format_.getDataSize());
    }

    void Array::fromStream(std::iostream& stream) {
      stream.read((char*)data_.get(), format_.getDataSize());
    }

    const data::Format& Array::getFormat() const {
      return format_;
    }

    std::ostream& operator<<(std::ostream& os, const Array& array) {
      return os << array.getFormat();
    }

  }
}
