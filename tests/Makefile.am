
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/source \
	-I$(top_srcdir)/source/lib \
	$(DEBUG_CPPFLAGS) 

AM_LDFLAGS = \
	$(top_srcdir)/source/lib/libblitzortung-tracker.la \
	-lboost_program_options \
	-lcppunit

noinst_PROGRAMS = test-lib \
	$(NULL)

test_lib_SOURCES = \
	test-lib.cc \
	test-lib-data-samples.cc test-lib-data-samples.h \
	test-lib-hardware-pcb.cc test-lib-hardware-pcb.h \
	$(NULL)

