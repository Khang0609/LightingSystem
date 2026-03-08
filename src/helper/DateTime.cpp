#include <helper/DateTime.h>
#include <chrono>
#include <iostream>
#include <format>

double getCurrentTimeInDecimal() {
  auto now = std::chrono::system_clock::now();
  std::time_t present = std::chrono::system_clock::to_time_t(now);
  std::tm local_time;

#ifdef _WIN32
  localtime_s(&local_time, &present);
#else
  localtime_r(&present, &local_time);
#endif

  return local_time.tm_hour + local_time.tm_min / 60.0 +
         local_time.tm_sec / 3600.0;
}

void getCurrentDate(int &day, int &month, int &year) {
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::tm *now_tm = std::localtime(&now_c);

  day = now_tm->tm_mday;
  month = now_tm->tm_mon + 1; // tm_mon chạy từ 0-11
  year = now_tm->tm_year + 1900;
};

void printFormattedTime(const std::string &label, double decimalTime) {

  if (decimalTime < 0) {
    std::cout << std::format("{}: N/A (Polar day/night)\n", label);
    return;
  }
  int hours = static_cast<int>(decimalTime);
  int minutes = static_cast<int>((decimalTime - hours) * 60);
  std::cout << std::format("{}: {:02d}h{:02d}p\n", label, hours, minutes);
}