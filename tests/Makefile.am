
AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/source \
	-I$(top_srcdir)/source/lib \
	$(DEBUG_CPPFLAGS) 

AM_LDFLAGS = \
	$(top_srcdir)/source/lib/libblitzortung.la \
	-lboost_program_options \
	-lcppunit

noinst_PROGRAMS = test-lib \
	$(NULL)

test_lib_SOURCES = \
	test-lib.cc \
	test-lib-data-events.cc test-lib-data-events.h \
	test-lib-data-events-V1.cc test-lib-data-events-V1.h \
	test-lib-data-events-V2.cc test-lib-data-events-V2.h \
	test-lib-data-waveform.cc test-lib-data-waveform.h \
	test-lib-hardware-pcb.cc test-lib-hardware-pcb.h \
	$(NULL)

