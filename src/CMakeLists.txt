project(Project_Detritus)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

set(SOURCE_LIST
    main.cpp
    application
)

add_subdirectory(graphic)
add_subdirectory(resource_manager)

include_directories(.)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_link_libraries(${PROJECT_NAME} graphic resource_manager)
