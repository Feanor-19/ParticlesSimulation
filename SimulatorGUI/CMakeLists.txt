cmake_minimum_required(VERSION 3.11)
project(SimulatorGUI)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(SRC_DIR src)

add_executable(SimulatorGUI ${SRC_DIR}/main.cpp 
                            ${SRC_DIR}/main_frame.cpp 
                            ${SRC_DIR}/particle_templates.cpp
                            ${SRC_DIR}/validators.cpp
                            ${SRC_DIR}/particle_canvas.cpp
                            ${SRC_DIR}/dlg_impl_params.cpp
                            ${SRC_DIR}/db_particles.cpp
                            ${SRC_DIR}/sim_gui_wrapper.cpp
                            ${SRC_DIR}/dlg_save_load.cpp)
target_include_directories(SimulatorGUI PUBLIC inc)

target_link_libraries(SimulatorGUI SimulatorCoreLib)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()
target_link_libraries(SimulatorGUI ${wxWidgets_LIBRARIES})

find_package(SQLite3)
target_link_libraries(SimulatorGUI SQLite::SQLite3)

find_package(SQLiteCpp)
target_link_libraries(SimulatorGUI SQLiteCpp)

# open MP
set(CMAKE_CXX_FLAGS "-fopenmp")