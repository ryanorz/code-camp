cmake_minimum_required(VERSION 2.8)

project(hbasecpp)
set(hbasecpp_VERSION_MAJOR 0)
set(hbasecpp_VERSION_MINOR 0)
set(hbasecpp_VERSION_PATCH 1)
set(hbasecpp_VERSION ${hbasecpp_VERSION_MAJOR}.${hbasecpp_VERSION_MINOR}.${hbasecpp_VERSION_PATCH})
set(PACKAGE_VERSION ${hbasecpp_VERSION})
set(VERSION "${hbasecpp_VERSION}")

include("${CMAKE_SOURCE_DIR}/cmake/configuration.cmake")

######################################################################
# Dependencies
######################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

######################################################################
# find librestbed, using for HTTP client
# ubuntu should install package:
#   librestbed-dev
######################################################################
find_package(restbed REQUIRED)
include_directories(${restbed_INCLUDE_DIR})
link_directories(${restbed_LIBRARY_DIRS})

######################################################################
# find protobuf library, and check protobuf-compiler
# ubuntu should install package:
#   protobuf-compiler
#   libprotobuf-dev
######################################################################
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
execute_process(
  COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --version
  OUTPUT_VARIABLE PROTOBUF_VERSION
)
string(REGEX MATCH "([0-9.]+).*" VERSION_NUMBER ${PROTOBUF_VERSION})
if(VERSION_NUMBER VERSION_LESS "3.0.0")
    message("protoc version < 3.0.0")
else()
    message("protoc version >= 3.0.0")
endif()
include_directories(${CMAKE_CURRENT_BINARY_DIR})

######################################################################
# compile hbase_rest proto cpp source and header
######################################################################
set(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB HBASE_REST_PROTOS "*.proto")

foreach(EACH_PROTO ${HBASE_REST_PROTOS})
    get_filename_component(EACH_NAME ${EACH_PROTO} NAME_WE)

    list(APPEND HBASE_REST_PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${EACH_NAME}.pb.cc")
    list(APPEND HBASE_REST_PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${EACH_NAME}.pb.h")

    execute_process(
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${EACH_PROTO}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

######################################################################
# compile hbasecpp test suite
######################################################################
add_executable(hbasecpp_test_suite
    ${HBASE_REST_PROTO_SRCS}
    hbasecpp.cc
    test_suite.cc
)
target_link_libraries(hbasecpp_test_suite
    ${PROTOBUF_LIBRARIES}
    ${restbed_LIBRARIES}
)
