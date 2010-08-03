#include "util/String.h"

namespace blitzortung {
  namespace util {

    void String::split(const std::string &input, std::vector<std::string> &results, const std::string &regexp) {

      results.clear();

      boost::regex re(regexp);
      boost::sregex_token_iterator i(input.begin(), input.end(), re, -1);
      boost::sregex_token_iterator end;

      while (i != end) {
	std::string field = *i++;
	results.push_back(field);
      }
    }

  }
}
