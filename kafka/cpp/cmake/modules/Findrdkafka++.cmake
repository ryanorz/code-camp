# - Try to find rdkafka++ ( librdkafka++.so )
# Once done, this will define
#
# rdkafka++_FOUND - system has rdkafka++
# rdkafka++_INCLUDE_DIR - the rdkafka++ include directories
# rdkafka++_LIBRARIES - link these to use rdkafka++

include("LibFindMacros")

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(rdkafka++_PKGCONF rdkafka++)

# Include dir
find_path(rdkafka++_INCLUDE_DIR
  NAMES librdkafka/rdkafkacpp.h
  PATHS ${rdkafka++_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(rdkafka++_LIBRARY
  NAMES rdkafka++
  PATHS ${rdkafka++_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(rdkafka++_PROCESS_INCLUDES rdkafka++_INCLUDE_DIR)
set(rdkafka++_PROCESS_LIBS rdkafka++_LIBRARY)
libfind_process(rdkafka++)

