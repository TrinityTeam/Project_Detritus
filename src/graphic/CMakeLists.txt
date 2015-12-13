cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

set(SOURCE_LIST
    node.cpp
    sprite.cpp
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(SFML 2 REQUIRED system window graphics network audio)
find_package(Box2D REQUIRED)


add_library(graphic ${SOURCE_LIST}) 

target_link_libraries(graphic ${SFML_LIBRARIES} ${Box2D_LIBRARIES})

