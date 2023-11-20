cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(GOMOKU LANGUAGES CXX)

file(COPY ${CMAKE_SOURCE_DIR}/test/scripts DESTINATION .)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()
if(MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(-Wno-error=maybe-uninitialized)  # Pour GCC ou Clang
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(PROJECT_NAME_GOMOKU pbrain-gomoku-ai)

add_executable(${PROJECT_NAME_GOMOKU})
set_target_properties(${PROJECT_NAME_GOMOKU} PROPERTIES ENABLE_EXPORTS ON)

add_subdirectory(src)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/release-1.10.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(gomoku_tests test/test_main.cpp test/gomoku_test_cases.cpp)
    target_link_libraries(gomoku_tests gtest_main ${PROJECT_NAME_GOMOKU})

    include(GoogleTest)
    gtest_discover_tests(gomoku_tests)
endif()