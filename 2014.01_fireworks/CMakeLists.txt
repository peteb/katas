cmake_minimum_required (VERSION 2.6)
project (Fireworks)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package (OpenGL REQUIRED)
find_package (GLFW REQUIRED)

if(APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework Cocoa -framework QuartzCore -framework IOKit -framework OpenGL")
endif()

add_executable (Fireworks main.cc)
target_link_libraries(Fireworks 
  ${LIBOPENGL_LIBRARIES} 
  ${GLFW_LIBRARY}
  ${COREFOUNDATION_LIBRARY}
)
include_directories(${LIBOPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})

