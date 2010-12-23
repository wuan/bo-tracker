#include "data/Array.h"

namespace blitzortung {
  namespace data {

    Array::Array(const Format& format) :
      format_(format)
    {
    }

    Array::~Array() {
    }

    unsigned int Array::getNumberOfSamples() const {
      return format_.getNumberOfSamples();
    }

    unsigned short Array::getNumberOfChannels() const {
      return format_.getNumberOfChannels();
    }

    void Array::set(int value, unsigned int sample, unsigned short channel) {
    }

    int Array::get(unsigned int sample, unsigned short channel) const {
      return 0;
    }

    void Array::toStream(std::iostream&) const {
    }

    void Array::fromStream(std::iostream&) {
    }

    const data::Format& Array::getFormat() const {
      return format_;
    }

    std::ostream& operator<<(std::ostream& os, const Array& array) {
      return os << array.getFormat();
    }

  }
}
