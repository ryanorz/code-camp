# - Try to find restbed ( librestbed.so )
# Once done, this will define
#
# restbed_FOUND - system has restbed
# restbed_INCLUDE_DIR - the restbed include directories
# restbed_LIBRARIES - link these to use restbed

include("LibFindMacros")

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(restbed_PKGCONF restbed)

# Include dir
find_path(restbed_INCLUDE_DIR
  NAMES restbed
  PATHS ${restbed_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(restbed_LIBRARY
  NAMES restbed restbed NAMES_PER_DIR
  PATHS ${restbed_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(restbed_PROCESS_INCLUDES restbed_INCLUDE_DIR)
set(restbed_PROCESS_LIBS restbed_LIBRARY restbed_dependency_LIBRARY)
libfind_process(restbed)
