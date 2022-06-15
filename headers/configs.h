//
// Created by wave on 2022/3/23.
//

#ifndef MESHPROCESSING_HEADERS_CONFIGS_H_
#define MESHPROCESSING_HEADERS_CONFIGS_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "json.hpp"

class Config {
 public:
  Config(const std::string& config_json_filepath);
  const std::string getMeshPath();
  const std::string getMeshName();
  const std::string getSavePath();
  const std::string getLogsPath();
  const double getStopRatio();
  const int getMethodType();

 private:
  std::string mesh_path;
  std::string savepath;
  std::string logs_folder_path;
  double ratio = 0.1;
  int method_type = -1;
};

#endif  // MESHPROCESSING_HEADERS_CONFIGS_H_
