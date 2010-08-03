#ifndef BLITZORTUNG_UTIL_STRING_H_
#define BLITZORTUNG_UTIL_STRING_H_

#include <string>

#include <boost/algorithm/string/regex.hpp>

namespace blitzortung {
  namespace util {

    class String {
      public:
	static void split(const std::string &input, std::vector<std::string> &results, const std::string &regexp);

    };

  }
}

#endif
