cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

set(SOURCE_LIST
    resource_manager.cpp
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(SFML 2 REQUIRED system window graphics network audio)


add_library(resource_manager ${SOURCE_LIST}) 

target_link_libraries(resource_manager ${SFML_LIBRARIES})

