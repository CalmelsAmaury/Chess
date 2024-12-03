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


# Add a post-build custom command to copy the file to the test directory
set(SOURCE_FILE_PATH "${CMAKE_SOURCE_DIR}/test/Chess.cfg")
 
 

add_custom_command(TARGET ChessLib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE_PATH} "$<TARGET_FILE_DIR:ChessLib>/Chess.cfg"
    COMMENT "Copying Chess.cfg to the test executable directory"
)
 
include(CTest)
enable_testing()
add_subdirectory(test)

