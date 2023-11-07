cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GOMOKU LANGUAGES CXX)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(PROJECT_NAME_GOMOKU pbrain-gomoku-ai)

add_executable(${PROJECT_NAME_GOMOKU})


add_subdirectory(src)
