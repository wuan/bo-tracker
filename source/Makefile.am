SUBDIRS = lib

bin_PROGRAMS = bo-tracker \
	       bo-data \
	       $(NULL)

AM_CPPFLAGS = \
	$(JSON_CFLAGS) \
	$(LOG4CPP_CFLAGS) \
	-I lib \
	$(NULL)

AM_LDFLAGS = \
	$(JSON_LIBS) \
        $(LOG4CPP_LIBS) \
        -lboost_system \
        -lboost_program_options \
	$(BOOST_MATH_LDFLAGS) \
	$(NULL)

bo_tracker_SOURCES = \
	bo-tracker.cc

bo_tracker_LDADD = \
	lib/libblitzortung.la

bo_data_SOURCES = \
	bo-data.cc

bo_data_LDADD = \
	lib/libblitzortung.la
