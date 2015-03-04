AUTOMAKE_OPTIONS = subdir-objects

AM_LDFLAGS = \
	$(NULL)

AM_CPPFLAGS = \
	$(JSON_CFLAGS) \
	$(LOG4CPP_CFLAGS) \
	$(NULL)

lib_LTLIBRARIES = libblitzortung.la

libblitzortung_la_LDFLAGS = \
	$(JSON_LIBS) \
	$(LOG4CPP_LIBS) \
	-lboost_regex \
        -lboost_system \
	-release ${VERSION} \
	-version_info ${LIBRARY_VERSION} \
	-no undefined \
	$(NULL)

libblitzortung_la_SOURCES = \
	DataWorker.h DataWorker.cc \
	\
	data/Array.h data/Array.cc \
	data/ArrayOf.h data/ArrayOf.cc \
	data/Format.h data/Format.cc \
	data/Event.h data/Event.cc \
	data/MEvent.h data/MEvent.cc \
	data/Events.h data/Events.cc \
	data/EventsFile.h data/EventsFile.cc \
	data/EventsHeader.h data/EventsHeader.cc \
	data/Waveform.h data/Waveform.cc \
	data/WaveformOf.h data/WaveformOf.cc \
	data/GpsInfo.h data/GpsInfo.cc \
	\
	exception/Base.h exception/Base.cc \
	\
	hardware/gps/Base.h hardware/gps/Base.cc \
	hardware/gps/Dummy.h hardware/gps/Dummy.cc \
	hardware/gps/Sirf.h hardware/gps/Sirf.cc \
	hardware/gps/Garmin.h hardware/gps/Garmin.cc \
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
	hardware/parsing/Base.h hardware/parsing/Base.cc \
	hardware/parsing/Ticks.h hardware/parsing/Ticks.cc \
	hardware/parsing/Samples.h hardware/parsing/Samples.cc \
	\
	hardware/Pcb.h hardware/Pcb.cc \
	\
	Logger.h Logger.cc \
	\
	namespace.h \
	\
	network/Creds.h network/Creds.cc \
	network/transfer/Base.h network/transfer/Base.cc \
	network/transfer/Udp.h network/transfer/Udp.cc \
	network/transfer/None.h network/transfer/None.cc \
	\
	output/Base.h \
	output/None.h output/None.cc \
	output/File.h output/File.cc \
	\
	Process.h Process.cc \
	\
	ipc/UnixSocket.h ipc/UnixSocket.cc \
	ipc/Listener.h ipc/Listener.cc \
	ipc/server/Base.h ipc/server/Base.cc \
	ipc/server/factory/Base.h ipc/server/factory/Base.cc \
	ipc/server/Json.h ipc/server/Json.cc \
	ipc/server/factory/Json.h ipc/server/factory/Json.cc \
	\
	Queue.h Queue.cc \
	\
	util/RingBuffer.h util/RingBuffer.cc \
	util/Size.h \
	util/String.h util/String.cc \
	util/Stream.h \
	\
	$(NULL)

