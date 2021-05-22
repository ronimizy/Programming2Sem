cmake_minimum_required(VERSION 3.17)
project(GeneticCube)

set(CMAKE_CXX_STANDARD 20)

set (URHO3D_HOME ${CMAKE_SOURCE_DIR}/Urho3D)
set (CMAKE_MODULE_PATH ${URHO3D_HOME}/CMake/Modules)
set (URHO3D_LIBRARIES ${URHO3D_HOME}/lib/Urho3D/Urho3D.a)

add_definitions(-DURHO_PREFIX="${URHO3D_HOME}/bin/")

include (UrhoCommon)

set (TARGET_NAME GeneticCube)
# Define source files
file (GLOB_RECURSE SOURCE
        Source/*.cpp
        Source/*.h
        Source/*.hpp
        main.cpp)

message(${SOURCE})

set (SOURCE_FILES ${SOURCE})

# Setup target with resource copying
setup_main_executable ()