#include <chrono>
#include <iostream>

#include "../headers/CLI11.hpp"
#include "../headers/configs.h"
#include "../headers/logger.h"
#include "../headers/mesh_edge_collapse.h"

using namespace std;

// Enum
enum class Simplify_method {
  GH_STANDARD = 0,
  LT_STANDARD = 1,
  LT_KEEP_BOUNDARY = 2,
  NUM_METHOD
};

int main(int argc, char **argv) {
  double ratio = 0.1;
  std::string json_file_path, mesh_path, save_path, logs_folder;
  int method = 0;
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
  app.add_option("-o,--output", save_path, "save result in obj file");
  app.add_option("-l,--logger", logs_folder, "logs folder path");
  app.add_option("-m, --method", method,
                 "simplify method, int type from 0 to 2");
  CLI11_PARSE(app, argc, argv);

  if (argc == 1) {
    json_file_path = "../configs/config.json";
  }

  if (!json_file_path.empty()) {
    std::cout << "load configs from config.json" << std::endl;
    try {
      Config config(json_file_path);
      mesh_path = config.getMeshPath();
      ratio = config.getStopRatio();
      save_path = config.getSavePath();
      method = config.getMethodType();
      logs_folder = config.getLogsPath();
    } catch (const std::exception &exc) {
      std::cerr << exc.what();
      return -1;
    }
  } else if (mesh_path.empty() || save_path.empty() || logs_folder.empty()) {
    std::cerr << "Error: Miss mesh_path or save_path or logger path"
              << std::endl;
    return -1;
  }

  logger_init(logs_folder);

  // mesh simplification pipeline
  SM surfacemesh;
  auto read_start = std::chrono::high_resolution_clock::now();
  bool isopen = CGAL::IO::read_polygon_mesh(mesh_path, surfacemesh);
  if (!isopen) {
    glogger->info("read polygon mesh failed");
    return -1;
  }
  auto read_end = std::chrono::high_resolution_clock::now();
  auto read_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      read_end - read_start);
  glogger->info("read mesh takes {} ms", read_duration.count());
  std::cout << "read mesh takes " << read_duration.count() << " ms"
            << std::endl;
  //  CGAL::draw(surfacemesh, "original mesh");
  glogger->info("simplify mesh : {}, ratio is {}, method is {}", mesh_path,
                ratio, method);

  switch (method) {
    case static_cast<int>(Simplify_method::GH_STANDARD):
      GH_QEM_standard(surfacemesh, ratio);
      break;
    case static_cast<int>(Simplify_method::LT_STANDARD):
      LT_standard(surfacemesh, ratio);
      break;
    case static_cast<int>(Simplify_method::LT_KEEP_BOUNDARY):
      LT_keep_boundary(surfacemesh, ratio);
      break;
    default:
      glogger->error("Choose method {} is not available, range is from 0 to {}",
                     method, static_cast<int>(Simplify_method::NUM_METHOD) - 1);
      std::cerr << "Choose method " << method
                << " is not available, range is from 0 to "
                << static_cast<int>(Simplify_method::NUM_METHOD) - 1
                << std::endl;
      return -1;
  }

  CGAL::IO::write_polygon_mesh(save_path, surfacemesh,
                               CGAL::parameters::stream_precision(17));
  glogger->info("save result in {}", save_path);
  return 0;
}
