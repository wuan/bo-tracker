add_subdirectory(lib)

find_package(Boost)
find_package(PkgConfig)
pkg_check_modules(JSON json-c)

include_directories(${JSON_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
add_executable(bo-tracker bo-tracker.cc)
add_executable(bo-data bo-data.cc)

target_link_libraries (bo-tracker LINK_PUBLIC libblitzortung)
target_link_libraries (bo-data LINK_PUBLIC libblitzortung)

message("Your C++ compiler supports these C++ features:")
foreach(i ${CMAKE_CXX_COMPILE_FEATURES})
  message("${i}")
endforeach()

set(needed_features
    cxx_strong_enums
    cxx_constexpr
    cxx_auto_type)
target_compile_features(bo-tracker PRIVATE ${needed_features})
target_compile_features(bo-data PRIVATE ${needed_features})

