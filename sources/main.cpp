#include <chrono>
#include <iostream>

#include "../headers/CLI11.hpp"
#include "../headers/configs.h"
#include "../headers/logger.h"
#include "../headers/mesh_edge_collapse.h"

using namespace std;

int main(int argc, char **argv) {
  logger_init();

  double ratio = 0.1;
  std::string json_file_path;
  std::string mesh_path;
  std::string savepath;
  CLI::App app{
      "Mesh Simplification: if no params passed in, default execution is -c "
      "../configs/config.json"};
  app.add_option("-c,--config", json_file_path,
                 "i.e. ../configs/config.json, if config is set, other options "
                 "will be ignored")
      ->check(CLI::ExistingFile);
  app.add_option("-r,--ratio", ratio,
                 "stop ratio, 0.0 < ratio < 1.0, default is 0.1")
      ->check(CLI::Range(0.0, 1.0));
  app.add_option("-i,--input", mesh_path,
                 "input mesh filepath, format supports obj,ply")
      ->check(CLI::ExistingFile);
  app.add_option("-o,--output", savepath, "save result in obj file");
  CLI11_PARSE(app, argc, argv);

  if (argc == 1) {
    json_file_path = "../configs/config.json";
  }
  if (!json_file_path.empty()) {
    std::cout << "load configs from config.json" << std::endl;
    Config config(json_file_path);
    mesh_path = config.getMeshPath();
    ratio = config.getStopRatio();
    savepath = config.getSavePath();
  } else if (mesh_path.empty() || savepath.empty()) {
    std::cerr << "Error: Miss mesh_path or save_path" << std::endl;
    return -1;
  }

  SM surfacemesh;
  bool isopen = CGAL::IO::read_polygon_mesh(mesh_path, surfacemesh);
  if (!isopen) {
    glogger->info("read polygon mesh failed");
    return -1;
  }
  std::cout << std::to_string(surfacemesh.num_vertices()) << std::endl;
  //  CGAL::draw(surfacemesh, "original mesh");
  glogger->info("simplify mesh : {}, ratio is {}", mesh_path, ratio);
  //  LT_standard(surfacemesh, ratio);
  GH_QEM_simplification(surfacemesh, ratio);
  //  LH_keep_boundary(surfacemesh, ratio);
  //  GH_keep_boundary(surfacemesh, ratio);
  //  CGAL::draw(surfacemesh, "simplification");
  std::cout << std::to_string(surfacemesh.num_vertices()) << std::endl;
  std::ofstream vertex_ofs;
  vertex_ofs.open(
      "/home/wave/CLionProjects/MeshProcessing/visualize/vertex_idx.txt",
      std::ios::out);
  for (auto vertex : surfacemesh.vertices()) {
    vertex_ofs << vertex << std::endl;
  }
  vertex_ofs.close();
  CGAL::IO::write_polygon_mesh(savepath, surfacemesh,
                               CGAL::parameters::stream_precision(17));
  glogger->info("save result in {}", savepath);
  return 0;
}
