set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME_GOMOKU} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${PROJECT_NAME_GOMOKU}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/GameBot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Error.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Board.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Move.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Cell.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LineConfig.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LineConfigHash.cpp
)

