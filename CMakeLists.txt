cmake_minimum_required(VERSION 3.21)
project(MeshProcessing)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT "/home/wave/DownloadLibs/boost_1_77_0")
find_package(Boost 1.77.0 EXACT REQUIRED)

set(CGAL_DIR /home/wave/DownloadLibs/cgal)
find_package(CGAL)

add_executable(MeshProcessing
        sources/main.cpp
        headers/json.hpp
        )

target_link_libraries(MeshProcessing CGAL::CGAL)