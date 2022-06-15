//
// Created by wave on 2022/4/15.
//

#ifndef MESHPROCESSING_HEADERS_LOGGER_H_
#define MESHPROCESSING_HEADERS_LOGGER_H_

#include <../spdlog/sinks/basic_file_sink.h>
#include <../spdlog/spdlog.h>

#include <chrono>
#include <cstdio>
#include <ctime>
#include <memory>

extern std::shared_ptr<spdlog::logger> glogger;

void logger_init(const std::string& log_folder_path);

std::tuple<int, int, int> get_current_year_month_day();

#endif  // MESHPROCESSING_HEADERS_LOGGER_H_
