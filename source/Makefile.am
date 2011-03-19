SUBDIRS = lib

bin_PROGRAMS = blitzortung-tracker \
	       blitzortung-data \
	       $(NULL)

AM_CXXFLAGS = \
	-I lib
#	$(QTCORE_CFLAGS)

AM_LDFLAGS = \
	-lboost_program_options

blitzortung_tracker_SOURCES = \
	blitzortung-tracker.cc

blitzortung_tracker_LDADD = \
	lib/libblitzortung.la

blitzortung_data_SOURCES = \
	blitzortung-data.cc

blitzortung_data_LDADD = \
	lib/libblitzortung.la
