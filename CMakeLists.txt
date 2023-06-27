cmake_minimum_required(VERSION 3.8.0)

project(
  MQTT_example
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB mqtt_headers
     "mqtt/*.h"
)                       

find_package(PahoMqttCpp REQUIRED)
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(mqtt_subscriber mqtt_subscriber.cpp ${mqtt_headers})
add_executable(mqtt_publisher mqtt_publisher.cpp ${mqtt_headers})

target_link_libraries(mqtt_subscriber PahoMqttCpp::paho-mqttpp3 ${Boost_LIBRARIES})
target_link_libraries(mqtt_publisher PahoMqttCpp::paho-mqttpp3 ${Boost_LIBRARIES})

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} '-g'")
