#include "DimmingEngine.h"
#include <algorithm>
#include <cmath>

DimmingEngine::DimmingEngine()
    : currentLevel(0.0f), targetLevel(0.0f), startLevel(0.0f), duration(1.0f),
      elapsed(0.0f) {}

// Hàm quan trọng nhất: Cập nhật giá trị theo thời gian thực
void DimmingEngine::update(float deltaTime) {
  if (elapsed < duration) {
    elapsed += deltaTime;
    float t = std::min(1.0f, elapsed / duration);

    float smoothT = t * t * (3.0f - 2.0f * t);
    float logT = (std::pow(logBase, smoothT) - 1.0f) / (logBase - 1.0f);
    currentLevel = startLevel + (targetLevel - startLevel) * logT;

    writeToHardware(currentLevel);
  }
}

// Hàm cài đặt thay đổi
void DimmingEngine::setTarget(float newTarget, float time) {
  if (newTarget != targetLevel) {
    startLevel = currentLevel;
    targetLevel = std::clamp(newTarget, 0.0f, 1.0f);
    duration = std::max(0.1f, time);
    elapsed = 0;
  }
}

void DimmingEngine::writeToHardware(float level) {
  // Hàm này sẽ được ghi đè tùy vào phần cứng (Arduino, ESP32...)
  // Hoặc bỏ trống nếu chỉ là lớp cơ sở logic
  (void)level; // Suppress unused parameter warning
}