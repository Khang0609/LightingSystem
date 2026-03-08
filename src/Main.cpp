#include <DimmingEngine.h>
#include <LightSchedule.h>
#include <LightingStrategy.h>
#include <algorithm>
#include <chrono>
#include <helper/DateTime.h>
#include <iostream>
#include <thread>

LightingStrategy solarStrategy;
DimmingEngine mainDimmer;

float sensorLevel = 0.0f;

SunCalculationProps SolarProps{
    .longitude = 105.8 * (PI / 180.0), // Kinh độ (Radians)
    .latitude = 21.0 * (PI / 180.0),   // Vĩ độ (Radians)
    .dayOfYear = 1,
    .timeZone = 7};

void loop() {
  double now = getCurrentTimeInDecimal();

  // 0. Tự động cập nhật Dawn/Sunset chỉ khi sang ngày mới
  int d, m, y;
  getCurrentDate(d, m, y);
  solarStrategy.updateDaily(SolarProps, d, m, y);

  // 1. Lấy mức sáng nền từ tính toán mặt trời
  float baseLevel = solarStrategy.calculateBaseLevel(now);

  // 2. Lấy mức sáng theo khung giờ (Dùng giá trị đã được updateDaily tính sẵn)
  float scheduleLevel = getScheduleLevel(now, solarStrategy.getDawnTime(),
                                         solarStrategy.getSunsetTime());

  // 3. Hòa quyện với Cảm biến (Ưu tiên mức cao nhất)
  float target = std::max({baseLevel, scheduleLevel, sensorLevel});

  // 3. Thực thi Dimming mượt mà và tiết kiệm điện (Hàm Log của cậu)
  mainDimmer.setTarget(target, 10.0f); // Chuyển cảnh trong 10 giây
  mainDimmer.update(0.1f);

  // Print current status for debugging
  std::cout << "Time: ";
  printFormattedTime("Now", now);
  std::cout << "Target Level: " << target << std::endl;
}

int main() {
  std::cout << "Lighting System Initialized." << std::endl;

  // Vòng lặp chạy thử nghiệm (5 lần)
  for (int i = 0; i < 5; ++i) {
    loop();
    std::this_thread::sleep_for(std::chrono::seconds(60));
  }

  return 0;
}
