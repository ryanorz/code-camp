cmake_minimum_required(VERSION 2.6)
project(cpp)

include("${CMAKE_SOURCE_DIR}/cmake/configuration.cmake")

######################################################################
# Dependencies
######################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(RdKafka REQUIRED)
find_package(rdkafka++ REQUIRED)

add_executable(rdkafka_producer rdkafka_producer.cc)
target_link_libraries(rdkafka_producer
    ${RDKAFKA_LIBRARY}
)

add_executable(rdkafkacpp_producer rdkafkacpp_producer.cc)
target_link_libraries(rdkafkacpp_producer
    ${rdkafka++_LIBRARIES}
)

add_executable(rdkafka_consumer_subscribe rdkafka_consumer_subscribe.cc)
target_link_libraries(rdkafka_consumer_subscribe
    ${RDKAFKA_LIBRARY}
)

add_executable(rdkafka_consumer_assignment rdkafka_consumer_assignment.cc)
target_link_libraries(rdkafka_consumer_assignment
    ${RDKAFKA_LIBRARY}
)

add_executable(rdkafkacpp_consumer_subscribe rdkafkacpp_consumer_subscribe.cc)
target_link_libraries(rdkafkacpp_consumer_subscribe
    ${rdkafka++_LIBRARIES}
)
