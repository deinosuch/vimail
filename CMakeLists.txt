cmake_minimum_required(VERSION 3.23)
project(vimail LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

find_package(mailio REQUIRED)

set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(vimail
    PRIVATE
    ${CURSES_LIBRARIES}
    spdlog::spdlog
    $<$<BOOL:${MINGW}>:ws2_32>
    mailio::mailio
)
