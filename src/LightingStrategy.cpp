#include <LightingStrategy.h>

void LightingStrategy::updateDaily(SunCalculationProps &props, int d, int m,
                                   int y) {
  // Chỉ tính toán lại khi ngày thay đổi (Ví dụ từ ngày 7 sang ngày 8)
  if (d != lastUpdateDay) {
    props.dayOfYear = getDayOfYear(d, m, y);
    this->dawnTime = calculateDawnTime(props);
    this->sunsetTime = calculateTwilightTime(props);
    this->lastUpdateDay = d;

    // In log để biết khi nào hệ thống cập nhật biểu đồ mặt trời (tùy chọn)
  }
}

void LightingStrategy::updateSolarData(double dawn, double sunset) {
  this->dawnTime = dawn;
  this->sunsetTime = sunset;
}

// Hàm quan trọng nhất: Quyết định mức sáng dựa trên "Giờ thực"
float LightingStrategy::calculateBaseLevel(double currentTime) {
  // 1. Sau khi mặt trời lặn (Bắt đầu tối)
  if (currentTime >= sunsetTime &&
      currentTime < sunsetTime + transitionPeriod) {
    float progress = (currentTime - sunsetTime) / transitionPeriod;
    return progress * 0.8f; // Tăng dần lên 80%
  }

  // 2. Trước khi mặt trời mọc (Bắt đầu sáng)
  if (currentTime >= dawnTime - transitionPeriod && currentTime < dawnTime) {
    float progress = (dawnTime - currentTime) / transitionPeriod;
    return progress * 0.8f; // Giảm dần về 0%
  }

  // 3. Ban ngày (Tắt đèn)
  if (currentTime >= dawnTime && currentTime < sunsetTime) {
    return 0.0f;
  }

  // 4. Đêm khuya (Giao cho Lập lịch và Cảm biến)
  return 0.8f;
}