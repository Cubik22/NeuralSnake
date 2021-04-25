cmake_minimum_required(VERSION 3.12)

project(NeuralSnake CXX C)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        src/main.cpp src/Matrix.h src/NeuralNetwork.h src/NeuralNetworkDeep.h
        src/Population.h src/PopulationDeep.h src/Snake.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL2)

target_link_libraries(${PROJECT_NAME} PUBLIC m SDL2-static)

if(WINDOWS)
    target_link_libraries(${PROJECT_NAME} PUBLIC SDL2main)
    add_definitions(-Dmain=SDL_main)
endif()

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/res
                ${CMAKE_CURRENT_BINARY_DIR}/res)
