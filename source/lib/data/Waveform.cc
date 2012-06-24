#include "data/Format.h"
#include "data/Waveform.h"
#include "util/Stream.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {

    Waveform::~Waveform() = default;

    unsigned int Waveform::GetSize(const data::Format& dataFormat) {
      util::Size size;

      long long int nanoseconds;
      size.add(nanoseconds);

      if (dataFormat.getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds;
	size.add(deltaNanoseconds);
      }

      // gps data size + two times the size of an element (for x and y value)
      return size.get() + dataFormat.getDataSize();
    }

    std::ostream& operator <<(std::ostream& os, const bo::data::Waveform& wfm) {
	os << "Waveform(" << wfm.getTimestamp() << ", dt " << wfm.getTimeDelta() << ")";
	return os;
    }

  }
}
