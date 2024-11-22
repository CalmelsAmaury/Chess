cmake_minimum_required(VERSION 3.15)
project(Chess VERSION 0.1.0 LANGUAGES C CXX)



set(SOURCES_FILES 
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(presentation)
add_subdirectory(model)
add_subdirectory(services)
include_directories(model)

add_library(ChessLib ${SOURCES_FILES})

include(CTest)
enable_testing()
add_subdirectory(test)

