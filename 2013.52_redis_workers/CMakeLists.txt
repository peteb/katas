cmake_minimum_required (VERSION 2.6)
project (RedisMailGenerator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package (HiRedis REQUIRED)


add_executable (RedisMailGenerator server.c)
target_link_libraries(RedisMailGenerator ${LIBHIREDIS_LIBRARIES})
include_directories(${LIBHIREDIS_INCLUDE_DIR})
