SUBDIRS = lib

bin_PROGRAMS = blitzortung-tracker \
	       blitzortung-data \
	       $(NULL)

AM_CXXFLAGS = \
	-I lib

AM_LDFLAGS = \
	-ljson \
        -lboost_regex \
	-lboost_program_options \
	-llog4cpp \
	$(BOOST_MATH_LDFLAGS) \
	$(NULL)

blitzortung_tracker_SOURCES = \
	blitzortung-tracker.cc

blitzortung_tracker_LDADD = \
	lib/libblitzortung.la

blitzortung_data_SOURCES = \
	blitzortung-data.cc

blitzortung_data_LDADD = \
	lib/libblitzortung.la
