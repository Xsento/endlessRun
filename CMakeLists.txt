cmake_minimum_required(VERSION 3.10)

project(endlessRun)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add exec
add_executable (${PROJECT_NAME} src/main.cpp)
