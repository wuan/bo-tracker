#include "data/Waveform.h"

namespace blitzortung {
  namespace data {

    template <typename T>
    Waveform<T>::Waveform(const pt::ptime& t0) :
      t0_(t0)
    {
    }

    template <typename T>
    Waveform<T>::~Waveform()
    {
    }

    template <typename T>
    void Waveform<T>::add(T xval, T yval) {
      float val = xval * xval + yval * yval;

      if (xdata_.size() == 0 || val > maxVal_) {
	maxVal_ = val;
	maxIndex_ = xdata_.size();
      }

      xdata_.push_back(xval);
      ydata_.push_back(yval);
    }


    template <typename T>
    int Waveform<T>::getMaxIndex() const {
      if (xdata_.size() ==0)
	throw exception::Base("data::Waveform::getMaxIndex() data arrays empty");

      return maxIndex_;
    }

    template <typename T>
    T Waveform<T>::getMaxX() const {
      if (xdata_.size() ==0)
	throw exception::Base("data::Waveform::getMaxX() data arrays empty");

      return xdata_[maxIndex_];
    }

    template <typename T>
    T Waveform<T>::getMaxY() const {
      if (ydata_.size() ==0)
	throw exception::Base("data::Waveform::getMaxY() data arrays empty");

      return ydata_[maxIndex_];
    }

    //! explicit instatiation of functions to be linked afterwards
    template class Waveform<double>;
    template class Waveform<float>;
    template class Waveform<long int>;
    template class Waveform<long unsigned int>;
    template class Waveform<int>;
    template class Waveform<unsigned int>;
    template class Waveform<short>;
    template class Waveform<unsigned short>;
    template class Waveform<char>;
    template class Waveform<unsigned char>;
	

  }
}
