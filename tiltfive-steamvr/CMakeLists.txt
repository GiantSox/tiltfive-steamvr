# CMakeList.txt : CMake project for tiltfive-steamvr, include source and define
# project specific logic here.
#

project ("tiltfive-steamvr")

cmake_minimum_required (VERSION 3.20)

file(GLOB MySources CONFIGURE_DEPENDS *.c *.h *.cpp)

add_library (tiltfive-steamvr SHARED ${MySources} )

# SteamVR requires all driver dll filenames are prefixed with "driver_"
set_target_properties(tiltfive-steamvr PROPERTIES PREFIX "driver_")

target_link_libraries(tiltfive-steamvr PRIVATE d3d11.lib "${CMAKE_SOURCE_DIR}/deps/TiltFive/lib/win64/TiltFiveNative.dll.if.lib")
target_include_directories(tiltfive-steamvr PRIVATE
	"${CMAKE_SOURCE_DIR}/deps/openvr/"
	"${CMAKE_SOURCE_DIR}/deps/TiltFive/include/")

install(TARGETS tiltfive-steamvr DESTINATION ../bin/win64)