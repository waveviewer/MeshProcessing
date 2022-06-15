//
// Created by wave on 2022/4/14.
//

#include "../headers/mesh_edge_collapse.h"

/**
 * @brief Apply standard Garland-Heckbert cost and placement strategy in edge
 * collapsing
 *
 * @param surface_mesh
 * @param ratio
 */
void GH_QEM_standard(SM &surface_mesh, const double &ratio) {
  glogger->info("GH_QEM_standard() start");
  SMS::Count_ratio_stop_predicate<SM> stop(ratio);
  GH_policies gh_policies(surface_mesh);
  const GH_cost &gh_cost = gh_policies.get_cost();
  const GH_placement &gh_placement = gh_policies.get_placement();
  Bounded_GH_placement placement(gh_placement);

  auto start = std::chrono::high_resolution_clock::now();
  int deleted_edges_num = SMS::edge_collapse(
      surface_mesh, stop,
      CGAL::parameters::get_cost(gh_cost).get_placement(gh_placement));
  auto end = std::chrono::high_resolution_clock::now();
  auto collapse_duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  glogger->info("GH_QEM_standard, {} edges collapsed in {} ms",
                deleted_edges_num, collapse_duration.count());
  std::cout << "GH_QEM_standard " << deleted_edges_num << " edges collapsed in "
            << collapse_duration.count() << "ms" << std::endl;
}

/**
 * @brief Apply standard Lindstrom-Turk cost and placement strategy in edge
 * collapsing
 *
 * @param surface_mesh
 * @param ratio
 */
void LT_standard(SM &surface_mesh, const double &ratio) {
  glogger->info("LT_standard() start");
  SMS::Count_ratio_stop_predicate<SM> stop(ratio);

  auto start = std::chrono::high_resolution_clock::now();
  int deleted_edges_num = SMS::edge_collapse(surface_mesh, stop);
  auto end = std::chrono::high_resolution_clock::now();
  auto collapse_duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  glogger->info("LT_standard simplification, {} edges collapsed in {} ms",
                deleted_edges_num, collapse_duration.count());
}

/**
 * @brief Apply standard Lindstrom-Turk cost and placement strategy in edge
 * collapsing, and mark boundary edges as unremovable
 *
 * @param surface_mesh
 * @param ratio
 */
void LT_keep_boundary(SM &surface_mesh, const double &ratio) {
  glogger->info("LH_keep_boundary() start");
  SM::Property_map<halfedge_descriptor,
                   std::pair<Kernel::Point_3, Kernel::Point_3>>
      constrained_halfedges;
  constrained_halfedges =
      surface_mesh
          .add_property_map<halfedge_descriptor,
                            std::pair<Kernel::Point_3, Kernel::Point_3>>(
              "h:vertices")
          .first;
  std::size_t num_border_edges = 0;
  for (halfedge_descriptor hd : halfedges(surface_mesh)) {
    if (CGAL::is_border(hd, surface_mesh)) {
      constrained_halfedges[hd] =
          std::make_pair(surface_mesh.point(source(hd, surface_mesh)),
                         surface_mesh.point(target(hd, surface_mesh)));
      ++num_border_edges;
    }
  }
  glogger->info("there are {} boundary edges in mesh", num_border_edges);

  // stop condition
  // Contract the surface mesh as much as possible
  SMS::Count_ratio_stop_predicate<SM> stop(ratio);
  glogger->info("set stop ratio is {}", ratio);

  Border_is_constrained_edge_map bem(surface_mesh);
  typedef SMS::Constrained_placement<SMS::Midpoint_placement<SM>,
                                     Border_is_constrained_edge_map>
      Placement;

  auto start = std::chrono::high_resolution_clock::now();
  int deleted_edges_num = SMS::edge_collapse(
      surface_mesh, stop,
      CGAL::parameters::edge_is_constrained_map(bem).get_placement(
          Placement(bem)));

  auto end = std::chrono::high_resolution_clock::now();
  auto collapse_duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  glogger->info("LH_keep_boundary simplification, {} edges collapsed in {} ms",
                deleted_edges_num, collapse_duration.count());
}

void GH_keep_boundary(SM &surface_mesh, const double &ratio) {
  glogger->info("GH_keep_boundary() start");
  SM::Property_map<halfedge_descriptor,
                   std::pair<Kernel::Point_3, Kernel::Point_3>>
      constrained_halfedges;
  constrained_halfedges =
      surface_mesh
          .add_property_map<halfedge_descriptor,
                            std::pair<Kernel::Point_3, Kernel::Point_3>>(
              "h:vertices")
          .first;
  std::size_t num_border_edges = 0;
  for (halfedge_descriptor hd : halfedges(surface_mesh)) {
    if (CGAL::is_border(hd, surface_mesh)) {
      constrained_halfedges[hd] =
          std::make_pair(surface_mesh.point(source(hd, surface_mesh)),
                         surface_mesh.point(target(hd, surface_mesh)));
      ++num_border_edges;
    }
  }
  glogger->info("there are {} boundary edges in mesh", num_border_edges);

  // stop condition
  SMS::Count_ratio_stop_predicate<SM> stop(ratio);
  glogger->info("set stop ratio is {}", ratio);

  Border_is_constrained_edge_map bem(surface_mesh);
  GH_policies gh_policies(surface_mesh);
  typedef SMS::Constrained_placement<GH_policies,
                                     Border_is_constrained_edge_map>
      Constrained_GH_Placement;

  const GH_cost &gh_cost = gh_policies.get_cost();
  const GH_placement &gh_placement = gh_policies.get_placement();
  // TODO: keep boundary in GH QEM
  //  Bounded_GH_placement placement(gh_placement);

  auto start = std::chrono::high_resolution_clock::now();

  //  int deleted_edges_num =
  //      SMS::edge_collapse(surface_mesh, stop,
  //                         CGAL::parameters::get_cost(gh_cost).get_placement(gh_placement));
  typedef SMS::Constrained_placement<GH_placement,
                                     Border_is_constrained_edge_map>
      Constrained_GH_placement;
  int deleted_edges_num = SMS::edge_collapse(
      surface_mesh, stop,
      CGAL::parameters::edge_is_constrained_map(bem).get_placement(
          Constrained_GH_placement(bem)));

  auto end = std::chrono::high_resolution_clock::now();
  auto collapse_duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  glogger->info("GH_keep_boundary simplification, {} edges collapsed in {} ms",
                deleted_edges_num, collapse_duration.count());

  // now check!
  for (halfedge_descriptor hd : halfedges(surface_mesh)) {
    if (CGAL::is_border(hd, surface_mesh)) {
      --num_border_edges;
      if (constrained_halfedges[hd] !=
          std::make_pair(surface_mesh.point(source(hd, surface_mesh)),
                         surface_mesh.point(target(hd, surface_mesh)))) {
        glogger->debug("oops. send us a bug report");
      }
    }
  }
}