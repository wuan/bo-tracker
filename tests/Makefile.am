
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/source \
	-I$(top_srcdir)/source/lib \
	$(DEBUG_CPPFLAGS) 

AM_LDFLAGS = \
	$(top_srcdir)/source/lib/libblitzortung.la \
	-lcppunit

bin_PROGRAMS = test-lib \
	$(NULL)

test_lib_SOURCES = \
	test-lib.cc \
	test-lib-data-samples.cc test-lib-data-samples.h \
	$(NULL)

