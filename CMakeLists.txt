cmake_minimum_required(VERSION 3.11)
project(ParticleSimulation)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(SimulatorCore)
add_subdirectory(SimulatorGUI)