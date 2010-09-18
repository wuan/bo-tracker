AUTOMAKE_OPTIONS = subdir-objects

AM_LDFLAGS = \
	-lboost_regex-mt \
	-lboost_thread-mt \
	-llog4cpp

lib_LTLIBRARIES = libblitzortung.la

libblitzortung_la_SOURCES = \
	data/Samples.h data/Samples.cc \
	data/SamplesFile.h data/SamplesFile.cc \
	data/SamplesHeader.h data/SamplesHeader.cc \
	data/Waveform.h data/Waveform.cc \
	\
	data/sample/Base.h data/sample/Base.cc \
	data/sample/V1.h data/sample/V1.cc \
	\
	exception/Base.h exception/Base.cc \
	\
	hardware/gps/Base.h hardware/gps/Base.cc \
	hardware/gps/Sirf.h hardware/gps/Sirf.cc \
	hardware/gps/Garmin.h hardware/gps/Garmin.cc \
	hardware/gps/Sjn.h hardware/gps/Sjn.cc \
	\
	hardware/gps/data/Base.h hardware/gps/data/Base.cc \
	hardware/gps/data/Time.h hardware/gps/data/Time.cc \
	hardware/gps/data/Location.h hardware/gps/data/Location.cc \
	\
	hardware/comm/Base.h hardware/comm/Base.cc \
	hardware/comm/Serial.h hardware/comm/Serial.cc \
	hardware/comm/Dummy.h hardware/comm/Dummy.cc \
	hardware/comm/SerialPort.h hardware/comm/SerialPort.cc \
	\
	hardware/pcb/Base.h hardware/pcb/Base.cc \
	hardware/pcb/V4.h hardware/pcb/V4.cc \
	hardware/pcb/V6.h hardware/pcb/V6.cc \
	\
	Logger.h Logger.cc \
	\
	namespace.h \
	\
	network/Base.h network/Base.cc \
	network/Transfer.h network/Transfer.cc \
	network/Queue.h network/Queue.cc \
	network/Creds.h network/Creds.cc \
	\
	util/RingBuffer.h util/RingBuffer.cc \
	util/String.h util/String.cc

