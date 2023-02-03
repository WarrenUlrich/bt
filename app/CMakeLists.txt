cmake_minimum_required(VERSION 3.10)

project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp")

# Create the static library and specify the source files, include directories, and libraries to link
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    bt
)