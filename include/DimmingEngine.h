#pragma once

class DimmingEngine {
private:
  float currentLevel;          // 0.0 - 1.0 (Mức vật lý xuất ra đèn)
  float targetLevel;           // Mức muốn đạt tới
  float startLevel;            // Mức bắt đầu khi nhận lệnh mới
  float duration;              // Thời gian chuyển cảnh (giây)
  float elapsed;               // Thời gian đã trôi qua
  const float logBase = 10.0f; // Cơ số để tối ưu tiết kiệm điện

public:
  DimmingEngine();

  void update(float deltaTime);
  void setTarget(float newTarget, float time);
  virtual void writeToHardware(float level);
};