

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1  # you can specify a commit, tag, or branch here
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


# Enable testing
enable_testing()

# Add the test executable
add_executable(TestChess test_algo.cpp)

# Link the test executable with the code under test
target_link_libraries(TestChess PRIVATE ChessLib gtest)

# Add the test to CTest
add_test(NAME TestChess COMMAND TestChess)


# Add a post-build custom command to copy the file to the test directory
set(SOURCE_FILE_PATH "${CMAKE_SOURCE_DIR}/test/Chess.cfg")
 
 

add_custom_command(TARGET TestChess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE_PATH} "$<TARGET_FILE_DIR:TestChess>/Chess.cfg"
    COMMENT "Copying Chess.cfg to the test executable directory"
)
 

 