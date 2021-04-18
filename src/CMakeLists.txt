cmake_minimum_required(VERSION 3.16)
project(TrainTicket)

set(CMAKE_CXX_STANDARD 14)

add_executable(TrainTicket main.cpp logger.h BasicHeader.h vector.hpp exceptions.hpp filemanip.h)
#add_executable(tester tester.cpp)