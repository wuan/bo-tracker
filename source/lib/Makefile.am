AUTOMAKE_OPTIONS = subdir-objects

AM_LDFLAGS = \
	-lboost_regex-mt \
	-lboost_thread-mt

lib_LTLIBRARIES = libblitzortung.la

libblitzortung_la_SOURCES = \
	data/Samples.h data/Samples.cc \
	data/SamplesFile.h data/SamplesFile.cc \
	data/sample/Base.h data/sample/Base.cc \
	data/sample/Version1.h data/sample/Version1.cc \
	\
	exception/Base.h exception/Base.cc \
	\
	hardware/gps/Base.h hardware/gps/Base.cc \
	hardware/gps/data/Base.h hardware/gps/data/Base.cc \
	hardware/gps/data/Time.h hardware/gps/data/Time.cc \
	hardware/gps/data/Location.h hardware/gps/data/Location.cc \
	hardware/Communication.h hardware/Communication.cc \
	hardware/SerialPort.h hardware/SerialPort.cc \
	hardware/pcb/Base.h hardware/pcb/Base.cc \
	hardware/pcb/V6.h hardware/pcb/V6.cc \
	\
	namespace.h \
	network/Base.h network/Base.cc \
	network/Creds.h network/Creds.cc \
	\
	util/RingBuffer.h util/RingBuffer.cc \
	util/String.h util/String.cc

