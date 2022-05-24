//
// Created by wave on 2022/3/23.
//

#ifndef MESHPROCESSING_SOURCES_CONFIGS_H_
#define MESHPROCESSING_SOURCES_CONFIGS_H_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "json.hpp"

class Config{
 public:
  Config(const std::string& config_json_filepath);
  const std::string getMeshPath();
  const std::string getMeshName();
  const std::string getSavePath();
  const double getStopRatio();
 private:
  std::string mesh_path;
  std::string savepath;
  double ratio = 0.1;
};

#endif //MESHPROCESSING_SOURCES_CONFIGS_H_
