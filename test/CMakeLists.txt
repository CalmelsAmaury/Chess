

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

add_executable(TestBack test_back.cpp)

# Link the test executable with the code under test
target_link_libraries(TestChess PRIVATE ChessLib gtest)
target_link_libraries(TestBack PRIVATE ChessLib gtest)

# Add the test to CTest
add_test(NAME TestChess COMMAND TestChess)
add_test(NAME TestBack COMMAND TestBack)


set(CHESS_CONFIGS Chess.cfg Chess2.cfg Chess3.cfg Chess4.cfg ChessInitialPosition.cfg)

# Boucle pour copier chaque fichier de configuration
foreach(CONFIG_FILE ${CHESS_CONFIGS})
    set(SOURCE_FILE_PATH "${CMAKE_SOURCE_DIR}/test/${CONFIG_FILE}")
    
    # Copie vers le répertoire de build
    add_custom_command(TARGET TestChess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE_PATH} "${CMAKE_BINARY_DIR}/test/${CONFIG_FILE}"
        COMMENT "Copying ${CONFIG_FILE} to the test working directory"
    )

    # Copie vers le répertoire de l'exécutable
    add_custom_command(TARGET TestChess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE_PATH} "$<TARGET_FILE_DIR:TestChess>/${CONFIG_FILE}"
        COMMENT "Copying ${CONFIG_FILE} to the test executable directory"
    )
endforeach()
 

 