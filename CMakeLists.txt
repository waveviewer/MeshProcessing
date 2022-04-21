cmake_minimum_required(VERSION 3.21)
project(MeshProcessing)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

set(BOOST_ROOT "/home/wave/DownloadLibs/boost_1_77_0")
find_package(Boost 1.77.0 EXACT REQUIRED)

set(CGAL_DIR /home/wave/DownloadLibs/cgal)
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

add_library(spdlog INTERFACE)
target_include_directories(
        spdlog INTERFACE
        "${CMAKE_SOURCE_DIR}"
)

find_package(Threads)

add_executable(MeshProcessing
        sources/main.cpp
        headers/json.hpp
        sources/configs.cpp headers/configs.h
        sources/mesh_edge_collapse.cpp headers/mesh_edge_collapse.h
        headers/logger.h sources/logger.cpp
#        sources/cgal_demo.cpp headers/cgal_demo.h
        )


target_link_libraries(MeshProcessing CGAL::CGAL CGAL::CGAL_Basic_viewer Eigen3::Eigen)
target_link_libraries(MeshProcessing spdlog ${CMAKE_THREAD_LIBS_INIT})