project("Project Detritus")

cmake_minimum_required (VERSION 2.8.6)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Wno-strict-aliasing")

if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

include (Urho3D-CMake-common)

find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

set(TARGET_NAME "Project Detritus")

define_source_files(EXTRA_CPP_FILES application character obstacle)
set_output_directories("${CMAKE_SOURCE_DIR}/../build")
setup_main_executable()

