cmake_minimum_required(VERSION 3.17)

project(Pre_Project VERSION 1.0)

set(CMAKE_CXX_STANDARD_11 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project("Pre_Project")

add_executable("${PROJECT_NAME}" Arm_Master.cxx Arm_Slave.cxx)

