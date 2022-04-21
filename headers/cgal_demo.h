//
// Created by wave on 2022/4/21.
//

#ifndef MESHPROCESSING_SOURCES_CGAL_DEMO_H_
#define MESHPROCESSING_SOURCES_CGAL_DEMO_H_


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
// Simplification function
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
// Stop-condition policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <iostream>
#include <fstream>
typedef CGAL::Simple_cartesian<double>                      Kernel;
typedef CGAL::Polyhedron_3<Kernel>                          Surface_mesh;
namespace SMS = CGAL::Surface_mesh_simplification;

#endif //MESHPROCESSING_SOURCES_CGAL_DEMO_H_
