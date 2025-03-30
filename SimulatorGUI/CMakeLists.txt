cmake_minimum_required(VERSION 3.11)
project(SimulatorGUI)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(SRC_DIR src)

add_executable(SimulatorGUI ${SRC_DIR}/main.cpp)
target_include_directories(SimulatorGUI PUBLIC inc)

target_link_libraries(SimulatorGUI SimulatorCoreLib)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()
target_link_libraries(SimulatorGUI ${wxWidgets_LIBRARIES})
