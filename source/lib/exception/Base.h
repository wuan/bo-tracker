#ifndef BLITZORTUNG_EXCEPTION_BASE_H_
#define BLITZORTUNG_EXCEPTION_BASE_H_

#include <string>
#include <exception>

namespace blitzortung {
  namespace exception {

    class Base : public std::exception {
      protected:
	std::string msg_;

	Base() throw();

      public:
	Base(const std::string&) throw();
	virtual ~Base() throw();

	//! return the error description
	virtual const char* what() const throw();
    };
  }
}

#endif /* BASE_H_ */
