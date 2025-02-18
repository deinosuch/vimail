cmake_minimum_required(VERSION 3.23)
project(vimail LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

find_package(Boost REQUIRED COMPONENTS system thread date_time regex)
find_package(OpenSSL REQUIRED)
find_package(mailio REQUIRED)
find_package(Curses REQUIRED)
find_package(spdlog REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(
  vimail PRIVATE ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR}
                 ${CURSES_INCLUDE_DIR})

target_link_libraries(
  vimail
  PRIVATE ${Boost_LIBRARIES}
          ${CURSES_LIBRARY}
          OpenSSL::SSL
          OpenSSL::Crypto
          pthread
          mailio
          spdlog::spdlog)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Configuring Debug Build")
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
  target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
endif()
