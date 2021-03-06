dnl Process this file with autoconf to produce a configure script.

AC_INIT([blitzortung-tracker],[1.0.8])

dnl Every other copy of the package version number gets its value from here
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_MACRO_DIR([m4])

dnl create a config.h file (Automake will add -DHAVE_CONFIG_H)
AC_CONFIG_HEADERS([config.h])

CXXFLAGS="${CXXFLAGS} -g -std=gnu++0x -pthread"
CXXFLAGS="${CXXFLAGS} -fsigned-char -Wall -Wextra -pedantic -Weffc++ -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG -Wl,-E"
AC_SUBST(CXXFLAGS)

AC_PROG_CXX
# include libtool support
LT_INIT([pic-only, disable-static])

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/date_time.hpp], [], [AC_MSG_ERROR(You need the boost date-time libraries)])
AC_CHECK_HEADERS([boost/regex.hpp], [], [AC_MSG_ERROR(You need the boost regex libraries)])
AC_CHECK_HEADERS([boost/program_options.hpp], [], [AC_MSG_ERROR(You need the boost program-options libraries)])
AC_CHECK_HEADERS([boost/accumulators/accumulators.hpp], [AC_SUBST([BOOST_MATH_LDFLAGS], [])])
PKG_CHECK_MODULES([LOG4CPP], [log4cpp], [], [AC_MSG_ERROR(You need the log4cpp libraries)])
PKG_CHECK_MODULES([CPPUNIT], [cppunit], [], [AC_MSG_ERROR(You need the cppunit libraries)])
PKG_CHECK_MODULES([JSON], [json], [], [AC_MSG_ERROR(You need the json0 libraries)])
AC_LANG_POP([C++])

AC_CONFIG_FILES([\
	Makefile \
	source/Makefile \
	source/lib/Makefile \
	scripts/Makefile \
	tests/Makefile
    ])

AC_OUTPUT
