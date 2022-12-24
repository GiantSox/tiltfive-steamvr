# CMakeList.txt : CMake project for tiltfive-steamvr, include source and define
# project specific logic here.
#

project ("tiltfive-steamvr")

cmake_minimum_required (VERSION 3.8)

FILE(GLOB MySources *.c *.h *.cpp )

add_library (tiltfive-steamvr SHARED ${MySources} )

# SteamVR requires all driver dll filenames are prefixed with "driver_"
set_target_properties(tiltfive-steamvr PROPERTIES PREFIX "driver_")

link_directories(${CMAKE_SOURCE_DIR}/../deps/TiltFive/lib/win64)
target_link_libraries(tiltfive-steamvr d3d11.lib TiltFiveNative.dll.if.lib)

install(TARGETS tiltfive-steamvr DESTINATION ../bin/win64)