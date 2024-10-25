cmake_minimum_required(VERSION 3.22)
project(Minesweeper)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

# CMAKE_LINK_DIRECTORIES_BEFORE = ON

include_directories(./ ../)
if(APPLE)
    link_directories(BEFORE ./lib/MacOS)
elseif(UNIX)
    link_directories(./lib/Linux)
endif(APPLE)

add_executable(Minesweeper src/main.cpp src/Minesweeper.cpp src/SFMLUserInput.cpp src/KeyboardUserInput.cpp src/RandomEngine.cpp)

if(APPLE)
    target_link_libraries(Minesweeper
        libsfml-system.2.6.1.dylib
        libsfml-window.2.6.1.dylib
        libsfml-graphics.2.6.1.dylib
        libsfml-network.2.6.1.dylib
	libsfml-audio.2.6.1.dylib
    )
elseif(UNIX)
    target_link_libraries(Minesweeper
        libsfml-system.so.2.6.1
        libsfml-window.so.2.6.1
        libsfml-graphics.so.2.6.1
        libsfml-network.so.2.6.1
	libsfml-audio.so.2.6.1
    )
endif(APPLE)

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)


target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})

add_executable(${PROJECT_NAME}-ut tests/Minesweeper-ut.cpp src/Minesweeper.cpp)
target_compile_options(${PROJECT_NAME}-ut PRIVATE ${FLAGS})

if(APPLE)
    target_link_libraries(${PROJECT_NAME}-ut PUBLIC gtest_main gmock_main
        libsfml-system.2.6.1.dylib
        libsfml-window.2.6.1.dylib
        libsfml-graphics.2.6.1.dylib
        libsfml-network.2.6.1.dylib
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}-ut PUBLIC gtest_main gmock_main
        libsfml-system.so.2.6.1
        libsfml-window.so.2.6.1
        libsfml-graphics.so.2.6.1
        libsfml-network.so.2.6.1
    )
endif(APPLE)

add_test(NAME MinesweeperTests COMMAND ${PROJECT_NAME}-ut)