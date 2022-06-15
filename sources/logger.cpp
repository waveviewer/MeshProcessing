//
// Created by wave on 2022/4/15.
//

#include "../headers/logger.h"

std::shared_ptr<spdlog::logger> glogger = nullptr;

std::tuple<int, int, int> get_current_year_month_day() {
  auto now = std::chrono::system_clock::now();
  std::time_t date = std::chrono::system_clock::to_time_t(now);
  tm local_date = *localtime(&date);
  int year = 1900 + local_date.tm_year;
  int month = 1 + local_date.tm_mon;
  int day = local_date.tm_mday;
  return std::make_tuple(year, month, day);
}

void logger_init(const std::string& log_folder_path) {
  int year, month, day;
  std::tie(year, month, day) = get_current_year_month_day();
  char buffer[200];
  std::sprintf(buffer, "%s/%4d%02d%02d.txt", log_folder_path.c_str(), year,
               month, day);
  glogger = spdlog::basic_logger_mt("basic_logger", buffer);
  glogger->flush_on(spdlog::level::info);
  glogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}
