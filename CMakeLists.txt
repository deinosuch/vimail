cmake_minimum_required(VERSION 3.23)
project(vimail LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

find_package(mailio REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(vimail
    PRIVATE
    ${CURSES_LIBRARIES}
    spdlog::spdlog
    $<$<BOOL:${MINGW}>:ws2_32>
    mailio::mailio
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()
