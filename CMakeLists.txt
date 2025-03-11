cmake_minimum_required(VERSION 3.31)

project(saiyans LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} src include)

file(GLOB SRC_FILES src/*.c)

add_executable(saiyans ${SRC_FILES})

target_link_libraries(saiyans PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)

