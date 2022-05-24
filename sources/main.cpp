#include <iostream>
#include <chrono>
#include "../headers/mesh_edge_collapse.h"
#include "../headers/configs.h"
#include "../headers/logger.h"

using namespace std;

int main() {
  logger_init();

  Config config("../configs/config.json");
  std::string mesh_path = config.getMeshPath();
  SM surfacemesh;
  CGAL::IO::read_polygon_mesh(mesh_path, surfacemesh);
  double ratio = 0.1;

//  CGAL::draw(surfacemesh);
  // glogger->info("simplify mesh : {}, ratio is {}", mesh_path, ratio);
//  LT_standard(surfacemesh, ratio);
//  GH_QEM_simplification(surfacemesh, ratio);
  // LH_keep_boundary(surfacemesh, ratio);
  CGAL::draw(surfacemesh);
  // std::string savepath = config.getSavePath();
  // CGAL::IO::write_polygon_mesh(savepath, surfacemesh, CGAL::parameters::stream_precision(17));
  // glogger->info("save result in {}", savepath);
  return 0;
}
