//
// Created by wave on 2022/4/14.
//

#ifndef MESHPROCESSING_SOURCES_MESH_EDGE_COLLAPSE_H_
#define MESHPROCESSING_SOURCES_MESH_EDGE_COLLAPSE_H_

#include "../headers/logger.h"

// IO and Kernel
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/IO/polygon_mesh_io.h>

// Visualization
#include <CGAL/draw_surface_mesh.h>

// Simplification
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>
// Midpoint placement policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
//Placement wrapper
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_placement.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3> SM;
typedef boost::graph_traits<SM>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<SM>::edge_descriptor edge_descriptor;

namespace SMS = CGAL::Surface_mesh_simplification;

// Garland QEM
typedef typename SMS::GarlandHeckbert_policies<SM, Kernel> GH_policies;
typedef typename GH_policies::Get_cost GH_cost;
typedef typename GH_policies::Get_placement GH_placement;
typedef SMS::Bounded_normal_change_placement<GH_placement> Bounded_GH_placement;

void GH_QEM_simplification(SM &surface_mesh, const double &ratio);

void LT_standard(SM &surface_mesh, const double &ratio);

void LH_keep_boundary(SM &surface_mesh, const double &ratio);

// BGL property map which indicates whether an edge is marked as non-removable
struct Border_is_constrained_edge_map {
  const SM *sm_ptr;
  typedef edge_descriptor key_type;
  typedef bool value_type;
  typedef value_type reference;
  typedef boost::readable_property_map_tag category;
  Border_is_constrained_edge_map(const SM &sm) : sm_ptr(&sm) {}
  friend value_type get(const Border_is_constrained_edge_map &m, const key_type &edge) {
    return CGAL::is_border(edge, *m.sm_ptr);
  }
};

// Placement class
typedef SMS::Constrained_placement<SMS::Midpoint_placement<SM>, Border_is_constrained_edge_map> Placement;

#endif //MESHPROCESSING_SOURCES_MESH_EDGE_COLLAPSE_H_
