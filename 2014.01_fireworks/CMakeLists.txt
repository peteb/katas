cmake_minimum_required (VERSION 2.6)
project (Fireworks)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package (OpenGL REQUIRED)
find_package (SFML REQUIRED)


add_executable (Fireworks main.cc)
target_link_libraries(Fireworks ${LIBOPENGL_LIBRARIES})
include_directories(${LIBOPENGL_INCLUDE_DIR})
