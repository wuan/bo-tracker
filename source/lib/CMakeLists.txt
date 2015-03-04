cmake_minimum_required(VERSION 3.1)

find_package(PkgConfig)
pkg_check_modules(JSON QUIET json-c)
pkg_check_modules(LOG4CPP QUIET log4cpp)

include_directories(
    .
    data
    exception
    hardware
    ipc
    network
    output
    util
    ${JSON_INCLUDE_DIRS}
    ${LOG4CPP_INCLUDE_DIRS}
)

set(lib_src
    DataWorker
    Logger
    namespaces
    Process
    Queue
    data/Array
    data/ArrayOf
    data/Event
    data/Events
    data/EventsFile
    data/EventsHeader
    data/Format
    data/Gps
    data/GpsInfo
    data/MEvent
    data/Waveform
    data/WaveformOf
    exception/Base
    hardware/comm/Base
    hardware/comm/Dummy
    hardware/comm/Serial
    hardware/comm/SerialPort
    hardware/gps/data/Base
    hardware/gps/data/Location
    hardware/gps/data/Time
    hardware/gps/Base
    hardware/gps/Dummy
    hardware/gps/Garmin
    hardware/gps/Sirf
    hardware/parsing/Base
    hardware/parsing/Samples
    hardware/parsing/Ticks
    hardware/Pcb
    ipc/server/factory/Base
    ipc/server/factory/Json
    ipc/server/Base
    ipc/server/Json
    ipc/Listener
    ipc/UnixSocket
    network/transfer/Base
    network/transfer/None
    network/transfer/Udp
    network/Creds
    output/Base
    output/File
    output/None
    util/RingBuffer
    util/Size
    util/Stream
    util/String
)

add_library(libblitzortung SHARED ${lib_src})
#set_target_properties(libblitzortung PROPERTIES LINKER_LANGUAGE C++)

target_compile_features(libblitzortung PRIVATE ${needed_features})
