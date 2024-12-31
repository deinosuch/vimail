cmake_minimum_required(VERSION 3.15)
project(vimail LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
)
FetchContent_Declare(
    mailio
    GIT_REPOSITORY https://github.com/karastojko/mailio.git
    GIT_TAG 0.24.0
)
FetchContent_MakeAvailable(spdlog mailio)

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})

set(SOURCES main.cpp)

add_executable(vimail ${SOURCES})

target_link_libraries(vimail
    PRIVATE
    ${CURSES_LIBRARIES}
    spdlog::spdlog
    $<$<BOOL:${MINGW}>:ws2_32>
    mailio
)
