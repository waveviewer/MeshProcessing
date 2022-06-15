//
// Created by wave on 2022/4/14.
//

#ifndef MESHPROCESSING_HEADERS_MESH_EDGE_COLLAPSE_H_
#define MESHPROCESSING_HEADERS_MESH_EDGE_COLLAPSE_H_

#include "../headers/logger.h"

// IO and Kernel
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/IO/polygon_mesh_io.h>

// Visualization
#include <CGAL/draw_surface_mesh.h>

// Simplification
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_policies.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>

// Visitor base
#include <CGAL/Surface_mesh_simplification/Edge_collapse_visitor_base.h>

// Midpoint placement policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

// Placement wrapper
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_placement.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3> SM;
typedef boost::graph_traits<SM>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<SM>::edge_descriptor edge_descriptor;
typedef boost::graph_traits<SM>::vertex_descriptor vertex_descriptor;

namespace SMS = CGAL::Surface_mesh_simplification;

// Garland QEM
typedef typename SMS::GarlandHeckbert_policies<SM, Kernel> GH_policies;
typedef typename GH_policies::Get_cost GH_cost;
typedef typename GH_policies::Get_placement GH_placement;
typedef SMS::Bounded_normal_change_placement<GH_placement> Bounded_GH_placement;

void GH_QEM_standard(SM &surface_mesh, const double &ratio);

void LT_standard(SM &surface_mesh, const double &ratio);

void LT_keep_boundary(SM &surface_mesh, const double &ratio);

// void GH_keep_boundary(SM &surface_mesh, const double &ratio);

// BGL property map which indicates whether an edge is marked as non-removable
struct Border_is_constrained_edge_map {
  const SM *sm_ptr;
  typedef edge_descriptor key_type;
  typedef bool value_type;
  typedef value_type reference;
  typedef boost::readable_property_map_tag category;
  Border_is_constrained_edge_map(const SM &sm) : sm_ptr(&sm) {}
  friend value_type get(const Border_is_constrained_edge_map &m,
                        const key_type &edge) {
    return CGAL::is_border(edge, *m.sm_ptr);
  }
};

// UV property map and Visitor function
typedef SM::Property_map<vertex_descriptor, Kernel::Point_2> UV_pmap;
typedef SMS::Edge_profile<SM> Profile;
struct My_visitor : SMS::Edge_collapse_visitor_base<SM> {
  My_visitor(UV_pmap) : uv_pmap(uv_pmap) {}

  // Called during the processing phase for each edge being collapsed.
  // If placement is absent the edge is left uncollapsed.
  void OnCollapsing(const Profile &prof, boost::optional<Point> placement) {
    if (placement) {
      p0 = prof.p0();
      p1 = prof.p1();
      vertex_descriptor v0 = prof.v0();
      vertex_descriptor v1 = prof.v1();
      p0_2 = get(uv_pmap, v0);
      p1_2 = get(uv_pmap, v1);
      p_2 = CGAL::midpoint(p0_2, p1_2);
    }
  }

  // Called after each edge has been collapsed
  void OnCollapsed(const Profile &prof, vertex_descriptor vd) {
    put(uv_pmap, vd, p_2);
  }

  UV_pmap uv_pmap;
  Kernel::Point_3 p0, p1;
  Kernel::Point_2 p0_2, p1_2, p_2;
};

#endif  // MESHPROCESSING_HEADERS_MESH_EDGE_COLLAPSE_H_
