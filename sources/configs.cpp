//
// Created by wave on 2022/3/23.
//

#include "../headers/configs.h"

Config::Config(const std::string &config_json_filepath) {
  if (!std::filesystem::exists(config_json_filepath)) {
    throw std::runtime_error("Could not open " + config_json_filepath);
  }
  std::ifstream input_json_file(config_json_filepath);

  nlohmann::json config_json;
  try {
    config_json = nlohmann::json::parse(input_json_file);
  } catch (nlohmann::json::parse_error &ex) {
    std::cerr << "parse error at byte " << ex.byte << std::endl;
    throw std::runtime_error("Parsing json error");
  }

  if (config_json.contains("mesh path")) {
    mesh_path = config_json["mesh path"];
    if (!std::filesystem::exists(mesh_path)) {
      throw std::runtime_error(mesh_path + " does not exist");
    }
  } else {
    throw std::runtime_error("mesh path is missing in config file");
  }

  if (config_json.contains("save path")) {
    savepath = config_json["save path"];
  } else {
    throw std::runtime_error("save path is missing in config file");
  }

  if (config_json.contains("ratio")) {
    ratio = config_json["ratio"];
  } else {
    throw std::runtime_error("save path is missing in config file");
  }

  std::cout << "Init config file success !" << std::endl;
}

const std::string Config::getMeshPath() {
  return mesh_path;
}

const std::string Config::getMeshName() {
  std::filesystem::path path(mesh_path);
  return path.stem().string();
}

const std::string Config::getSavePath() {
  return savepath;
}

const double Config::getStopRatio() {
  return ratio;
}