cmake_minimum_required(VERSION 3.11)
project(SimulatorCore)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(SRC_DIR src)

add_library(SimulatorCoreLib ${SRC_DIR}/simulator.cpp 
                             ${SRC_DIR}/utilities.cpp 
                             ${SRC_DIR}/particles_state.cpp
                             ${SRC_DIR}/sim_manager.cpp
                             ${SRC_DIR}/impl_force_calc.cpp
                             ${SRC_DIR}/impl_integrator.cpp)
target_include_directories(SimulatorCoreLib PUBLIC inc)

add_executable(Driver ${SRC_DIR}/main.cpp)
target_include_directories(Driver PUBLIC inc)
target_link_libraries(Driver SimulatorCoreLib)

enable_testing()

# Unit tests with GTest
find_package(GTest REQUIRED)

add_executable(UnitTests ${SRC_DIR}/unit_tests.cpp)
target_include_directories(UnitTests PUBLIC inc)
target_link_libraries(UnitTests gtest::gtest)
target_link_libraries(UnitTests SimulatorCoreLib)

include(GoogleTest)
gtest_discover_tests(UnitTests)
