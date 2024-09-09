cmake_minimum_required(VERSION 3.20)
project(3500_E_3_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(3500_E_3_1 pthread-data-sharing-mutex-strict-alternation.cpp pthread-data-sharing-mutex.cpp pthread_join.cpp)
