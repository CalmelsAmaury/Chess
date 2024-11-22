cmake_minimum_required(VERSION 3.5.0)
project(Chess VERSION 0.1.0 LANGUAGES C CXX)



set(SOURCES_FILES 
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(presentation)
add_subdirectory(model)
add_subdirectory(services)

add_executable(Chess ${SOURCES_FILES})

include(CTest)
enable_testing()

