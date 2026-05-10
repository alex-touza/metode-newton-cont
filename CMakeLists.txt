cmake_minimum_required(VERSION 4.2)
project(metode_newton_cont)

set(CMAKE_CXX_STANDARD 20)

find_path(ADEPT_INCLUDE_DIR adept.h)
find_library(ADEPT_LIBRARY adept)

add_executable(metode_newton_cont src/newton_method_continuation.hpp src/main.cpp src/types.h)

if(ADEPT_INCLUDE_DIR AND ADEPT_LIBRARY)
    target_include_directories(metode_newton_cont PRIVATE adept)
    target_link_libraries(metode_newton_cont PRIVATE adept)
    message(STATUS "Adept found at: ${ADEPT_LIBRARY}")
else()
    message(FATAL_ERROR "Adept library not found!")
endif()