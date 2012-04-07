#include "data/ArrayOf.h"

namespace blitzortung {
  namespace data {

    template<typename T>
    ArrayOf<T>::ArrayOf(unsigned char numberOfChannels, unsigned short numberOfSamples) :
      numberOfChannels_(numberOfChannels)
    {
      data_.resize(numberOfChannels * numberOfSamples);
    }

    template<typename T>
    ArrayOf<T>::~ArrayOf() = default;

    template<typename T>
    unsigned int ArrayOf<T>::getNumberOfSamples() const {
      if (numberOfChannels_)
        return data_.size() / numberOfChannels_;
      else
	return 0;
    }

    template<typename T>
    unsigned short ArrayOf<T>::getNumberOfChannels() const {
      return numberOfChannels_;
    }

    template<typename T>
    int ArrayOf<T>::get(unsigned int sample, unsigned short channel) const {
      return data_[sample * numberOfChannels_ + channel];
    }

    template<typename T>
    float ArrayOf<T>::getFloat(unsigned int sample, unsigned short channel) const {
      return data_[sample * numberOfChannels_ + channel];
    }

    template<typename T>
    void ArrayOf<T>::set(int value, unsigned int sample, unsigned short channel) {
      data_[sample * numberOfChannels_ + channel] = value;
    }

    template<typename T>
    void ArrayOf<T>::set(float value, unsigned int sample, unsigned short channel) {
      data_[sample * numberOfChannels_ + channel] = value;
    }

    template<typename T>
    void ArrayOf<T>::toStream(std::iostream& stream) const {
      stream.write((char*)&data_, data_.size() * getElementSize());
    }

    template<typename T>
    void ArrayOf<T>::fromStream(std::iostream& stream) {
      stream.read((char*)&data_, data_.size() * getElementSize());
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const ArrayOf<T>& array) {
      return os << "ArrayOf(" << typeid(T).name() << ", " << array.getNumberOfChannels() << "ch, " << array.getNumberOfSamples() << " samples)";
    }

  }
}
