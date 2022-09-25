# CMakeList.txt : CMake project for tiltfive-steamvr, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

FILE(GLOB MySources *.c *.h *.cpp )

# Add source to this project's executable.
add_library (tiltfive-steamvr SHARED ${MySources} )

# TODO: Add tests and install targets if needed.
