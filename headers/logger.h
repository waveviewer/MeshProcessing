//
// Created by wave on 2022/4/15.
//

#ifndef MESHPROCESSING_HEADERS_LOGGER_H_
#define MESHPROCESSING_HEADERS_LOGGER_H_

#include <../spdlog/spdlog.h>
#include <../spdlog/sinks/basic_file_sink.h>

#include <memory>
#include <chrono>
#include <ctime>
#include <cstdio>

extern std::shared_ptr<spdlog::logger> glogger;

void logger_init();

std::tuple<int, int, int> get_current_year_month_day();

#endif //MESHPROCESSING_HEADERS_LOGGER_H_
