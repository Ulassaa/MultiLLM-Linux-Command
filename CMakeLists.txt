cmake_minimum_required(VERSION 3.10)
project(MultiLLM-Linux-Command)

set(CMAKE_CXX_STANDARD 17)

# Source files
file(GLOB SOURCES src/*.cpp src/llm/*.cpp src/utils/*.cpp)

add_executable(multillm ${SOURCES})

# Dependencies
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(jsoncpp REQUIRED)

include_directories(include)

# Link libraries
target_link_libraries(multillm PRIVATE CURL::libcurl Threads::Threads jsoncpp_lib)
