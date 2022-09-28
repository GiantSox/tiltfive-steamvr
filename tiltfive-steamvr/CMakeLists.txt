# CMakeList.txt : CMake project for tiltfive-steamvr, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

FILE(GLOB MySources *.c *.h *.cpp )

add_library (tiltfive-steamvr SHARED ${MySources} OvrHmd.cpp )

# SteamVR requires all driver dll filenames are prefixed with "driver_"
set_target_properties(tiltfive-steamvr PROPERTIES PREFIX "driver_")

install(TARGETS tiltfive-steamvr DESTINATION ../../../bin/win64)