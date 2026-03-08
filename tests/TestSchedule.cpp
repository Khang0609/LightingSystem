#include <LightSchedule.h>
#include <helper/DateTime.h>
#include <chrono>
#include <ctime>
#include <iostream>

int main() {
  double longitudeDeg, latitudeDeg;
  std::cout << "--- Lighting System Scheduler ---" << std::endl;
  std::cout << "Enter Longitude: ";
  if (!(std::cin >> longitudeDeg))
    return 1;
  std::cout << "Enter Latitude: ";
  if (!(std::cin >> latitudeDeg))
    return 1;

  double longitudeRad = longitudeDeg * PI / 180.0;
  double latitudeRad = latitudeDeg * PI / 180.0;

  auto now = std::chrono::system_clock::now();
  std::time_t present = std::chrono::system_clock::to_time_t(now);

  std::tm local_time;
  localtime_s(&local_time, &present);

  std::cout << "\n[System Info]" << std::endl;
  std::cout << "Local Date: " << local_time.tm_mday << "/"
            << local_time.tm_mon + 1 << "/" << local_time.tm_year + 1900
            << std::endl;

  int dayOfYear = getDayOfYear(local_time.tm_mday, local_time.tm_mon + 1,
                               local_time.tm_year + 1900);
  int timeZone = static_cast<int>(longitudeDeg / 15.0);

  SunCalculationProps props = {.longitude = longitudeRad,
                               .latitude = latitudeRad,
                               .dayOfYear = dayOfYear,
                               .timeZone = timeZone};

  std::cout << "\n[Calculation Results]" << std::endl;
  printFormattedTime("Sunrise (Dawn)", calculateDawnTime(props));
  printFormattedTime("Sunset (Twilight)", calculateTwilightTime(props));

  return 0;
}
