cmake_minimum_required(VERSION 2.8)
project(boost_example)

######################################################################
# Dependencies
######################################################################

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(program_options_example program_options_example.cc)
target_link_libraries(program_options_example
    ${Boost_LIBRARIES}
)

