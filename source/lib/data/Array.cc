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
	delete data_;
    }

    unsigned int Array::getNumberOfSamples() const {
      return format_->getNumberOfSamples();
    }

    unsigned short Array::getNumberOfChannels() const {
      return format_->getNumberOfChannels();
    }

    void Array::set(int value, unsigned int sample, unsigned short channel) {
      unsigned int index = format_->getIndex(sample, channel);
      data_[index] = char(value);
    }

    int Array::get(unsigned int sample, unsigned short channel) const {
      return data_[format_->getIndex(sample, channel)];
    }

    void Array::toStream(std::iostream& stream) const {
      stream.write(data_, format_->getDataSize());
    }

    void Array::fromStream(std::iostream& stream) {
      stream.read(data_, format_->getDataSize());
    }

    const data::Format::CP& Array::getFormat() const {
      return format_;
    }

    std::ostream& operator<<(std::ostream& os, const Array& array) {
      return os << array.getFormat();
    }

  }
}
