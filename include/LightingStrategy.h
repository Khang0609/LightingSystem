#pragma once

#include <LightSchedule.h>

class LightingStrategy {
private:
  double dawnTime = 0.0;
  double sunsetTime = 0.0;
  int lastUpdateDay = -1;              // Lưu ngày cập nhật cuối cùng
  const double transitionPeriod = 0.5; // 30 phút chuyển cảnh (đơn vị giờ)

public:
  // Tự động tính lại dawn/sunset nếu ngày đã thay đổi
  void updateDaily(SunCalculationProps &props, int day, int month, int year);

  void updateSolarData(double dawn, double sunset);

  // Hàm quan trọng nhất: Quyết định mức sáng dựa trên "Giờ thực"
  float calculateBaseLevel(double currentTime);

  double getDawnTime() const { return dawnTime; }
  double getSunsetTime() const { return sunsetTime; }
};