#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      Base::Base() {
      }

      Base::~Base() {
      }

      float Base::getAmplitude(int index) const {
	float xamp = getXAmplitude(index);
	float yamp = getYAmplitude(index);

	return sqrt(xamp * xamp + yamp * yamp);
      }

      bool Base::operator<(const Base &rhs) const {
	return getTime() < rhs.getTime();
      } 


      bool Base::CompareAmplitude::operator()(const first_argument_type &x, const second_argument_type &y) const {
	return x.getAmplitude(1) < y.getAmplitude(1);
      } 

      std::ostream& operator <<(std::ostream& os, const bo::data::sample::Base &sample) {

	pt::time_facet *timefacet = new pt::time_facet();
	timefacet->format("%Y%m%d %H:%M:%S.%f");
	std::locale oldLocale = os.imbue(std::locale(std::locale::classic(), timefacet));

	os << sample.getTime() << " " << sample.getAntennaLongitude() << " " << sample.getAntennaLatitude();
	os << " " << (int) sample.getGpsNumberOfSatellites();

	for (int peak=1; peak<=1; peak++) {
	  os << " " << sample.getTime(peak).time_of_day() << " " << sample.getAmplitude(peak);
	  os << " (" << sample.getXAmplitude(peak) << ", " << sample.getYAmplitude(peak) << ")";
	}

	// restore original locale
	os.imbue(oldLocale);

	return os;
      }

    }
  }
}
