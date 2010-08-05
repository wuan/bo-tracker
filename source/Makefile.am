SUBDIRS = lib

bin_PROGRAMS = blitzortung-tracker

AM_CXXFLAGS = \
	-I lib

AM_LDFLAGS = \
	-lboost_program_options

blitzortung_tracker_SOURCES = \
	blitzortung-tracker.cc

blitzortung_tracker_LDADD = \
	lib/libblitzortung.la
