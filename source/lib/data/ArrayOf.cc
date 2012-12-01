#include "data/Format.h"
#include "data/ArrayOf.h"

namespace blitzortung {
  namespace data {

    template<typename T, int numberOfSamples, int numberOfChannels>
      ArrayOf<T, numberOfSamples, numberOfChannels>::ArrayOf() :
      data_(numberOfChannels * numberOfSamples)
    {
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    ArrayOf<T, numberOfSamples, numberOfChannels>::~ArrayOf()
    {
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    Format ArrayOf<T, numberOfSamples, numberOfChannels>::getFormat() const {
      return Format(getElementSize(), numberOfChannels, numberOfSamples);
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    unsigned char ArrayOf<T, numberOfSamples, numberOfChannels>::getElementSize() const {
      return sizeof(T);
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    unsigned int ArrayOf<T, numberOfSamples, numberOfChannels>::getStorageSize() const {
      return data_.size() * getElementSize();
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    unsigned int ArrayOf<T, numberOfSamples, numberOfChannels>::getNumberOfSamples() const {
      if (numberOfChannels)
        return data_.size() / numberOfChannels;
      else
	return 0;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    unsigned short ArrayOf<T, numberOfSamples, numberOfChannels>::getNumberOfChannels() const {
      return numberOfChannels;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    bool ArrayOf<T, numberOfSamples, numberOfChannels>::isEmpty() const {
      return numberOfSamples == 0;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    T ArrayOf<T, numberOfSamples, numberOfChannels>::get(unsigned int sample, unsigned short channel) const {
      return data_[sample * numberOfChannels + channel];
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    int ArrayOf<T, numberOfSamples, numberOfChannels>::getInt(unsigned int sample, unsigned short channel) const {
      return (int)get(sample, channel);
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    float ArrayOf<T, numberOfSamples, numberOfChannels>::getFloat(unsigned int sample, unsigned short channel) const {
      return (float)get(sample, channel);
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    void ArrayOf<T, numberOfSamples, numberOfChannels>::set(T value, unsigned int sample, unsigned short channel) {
      data_[sample * numberOfChannels + channel] = value;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    void ArrayOf<T, numberOfSamples, numberOfChannels>::setInt(int value, unsigned int sample, unsigned short channel) {
      set(value, sample, channel);
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    void ArrayOf<T, numberOfSamples, numberOfChannels>::setFloat(float value, unsigned int sample, unsigned short channel) {
      set(value, sample, channel);
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    void ArrayOf<T, numberOfSamples, numberOfChannels>::toStream(std::iostream& stream) const {
      stream.write((char*)&data_[0], ArrayOf<T, numberOfSamples, numberOfChannels>::getStorageSize());
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    void ArrayOf<T, numberOfSamples, numberOfChannels>::fromStream(std::iostream& stream) {
      stream.read((char*)&data_[0], ArrayOf<T, numberOfSamples, numberOfChannels>::getStorageSize());
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    std::ostream& operator<<(std::ostream& os, const ArrayOf<T, numberOfSamples, numberOfChannels>& array) {
      return os << "ArrayOf(" << typeid(T).name() << ", " << array.getNumberOfChannels() << "ch, " << array.getNumberOfSamples() << " samples)";
    }

    //! explicit instatiation of functions to be linked afterwards
    template class ArrayOf<signed char, 0  , 1>;
    template class ArrayOf<signed char, 1  , 1>;
    template class ArrayOf<signed char, 64 , 1>;
    template class ArrayOf<signed char, 128, 1>;
    template class ArrayOf<signed char, 256, 1>;
    template class ArrayOf<signed char, 1  , 2>;
    template class ArrayOf<signed char, 64 , 2>;
    template class ArrayOf<signed char, 128, 2>;
    template class ArrayOf<signed char, 256, 2>;
    template class ArrayOf<signed short, 1, 2>;


  }
}
