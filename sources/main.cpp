#include <iostream>
#include "../headers/json.hpp"
#include <filesystem>
#include "map"
#include <CGAL/Surface_mesh.h>

using namespace nlohmann;
using namespace std::filesystem;
using namespace std;

int main() {
  std::ifstream input_json_file("../configs/config.json");
  json config;
  input_json_file >> config;
  cout << config["mesh path"] << endl;

  return 0;
}
