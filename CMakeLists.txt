cmake_minimum_required(VERSION 3.16)
project(redis_connector)

set(CMAKE_CXX_STANDARD 20)

find_package(redis++)

include(cmake-build-debug/conanbuildinfo.cmake)
conan_basic_setup()


add_executable(redis_connector main.cpp redisconnecter.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS} redis++_static)