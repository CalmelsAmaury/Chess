# Enable testing
enable_testing()

# Add the test executable
add_executable(TestChess test_algo.cpp)

# Link the test executable with the code under test
target_link_libraries(TestChess PRIVATE ChessLib)

# Add the test to CTest
add_test(NAME TestChess COMMAND TestChess)

