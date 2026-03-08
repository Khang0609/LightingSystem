#include <LightSchedule.h>
#include <cmath>

namespace {
// Shared constants for sun calculations
constexpr double SUN_DECLINATION_CONST = 0.39782;
constexpr double SUN_ZENITH_RAD = -0.01454; // -0.833 degrees in radians
constexpr double L_CONST_1 = 0.017202;
constexpr double L_CONST_2 = 0.0574;
constexpr double L_CONST_3 = 4.9348;
constexpr double RA_CONST = 0.91764;

struct SolarBaseParams {
  double RA;
  double SD;
};

double normalizeRadians(double rad) {
  double norm = std::fmod(rad, 2.0 * PI);
  if (norm < 0)
    norm += 2.0 * PI;
  return norm;
}

SolarBaseParams calculateSolarBase(int dayOfYear) {
  double M = L_CONST_1 * dayOfYear - L_CONST_2;
  double L =
      M + 0.0334 * std::sin(M) + 0.000349 * std::sin(2.0 * M) + L_CONST_3;
  L = normalizeRadians(L);

  double RA = std::atan(RA_CONST * std::tan(L));
  double PI2 = PI / 2.0;
  double Lquadrant = std::floor(L / PI2) * PI2;
  double RAquadrant = std::floor(RA / PI2) * PI2;
  RA = (RA + (Lquadrant - RAquadrant)) * 12.0 / PI;

  double SD = std::asin(SUN_DECLINATION_CONST * std::sin(L));

  return {RA, SD};
}

double calculateSunTime(const SunCalculationProps &p, bool isSunrise) {
  auto base = calculateSolarBase(p.dayOfYear);

  double cosH =
      (std::sin(SUN_ZENITH_RAD) - std::sin(p.latitude) * std::sin(base.SD)) /
      (std::cos(p.latitude) * std::cos(base.SD));

  // Check for polar day/night
  if (cosH > 1.0 || cosH < -1.0)
    return -1.0;

  double H = std::acos(cosH);
  if (isSunrise) {
    H = (2.0 * PI - H);
  }
  H = H * 12.0 / PI;

  double T = H + base.RA - 0.06571 * p.dayOfYear - 6.622;
  double UT = T - p.longitude * 12.0 / PI;
  double localHour = UT + p.timeZone;

  // Normalize to [0, 24)
  localHour = std::fmod(localHour, 24.0);
  if (localHour < 0)
    localHour += 24.0;

  return localHour;
}
} // namespace

int getDayOfYear(int day, int month, int year) {
  // Bảng cộng dồn số ngày trước mỗi tháng (năm không nhuận)
  static const int daysBeforeMonth[] = {0,   31,  59,  90,  120, 151,
                                        181, 212, 243, 273, 304, 334};

  int ordinal = daysBeforeMonth[month - 1] + day;

  // Kiểm tra năm nhuận nếu từ tháng 3 trở đi
  if (month > 2) {
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap)
      ordinal++;
  }

  return ordinal;
}

double calculateDawnTime(const SunCalculationProps &p) {
  return calculateSunTime(p, true);
}

double calculateTwilightTime(const SunCalculationProps &p) {
  return calculateSunTime(p, false);
}

float getScheduleLevel(double currentTime, double dawnTime, double sunsetTime) {
  // 1. Nếu đang trong thời gian ban ngày (giữa bình minh và hoàng hôn)
  if (currentTime >= dawnTime && currentTime < sunsetTime) {
    return 0.0f; // Trời sáng, tắt đèn theo lịch
  }

  // 2. Nếu là ban đêm, kiểm tra khung giờ cao điểm (18h - 22h)
  if (currentTime >= 18.0 && currentTime <= 22.0) {
    return 1.0f; // Giờ cao điểm, sáng tối đa
  }

  // 3. Các khung giờ ban đêm khác (đêm khuya hoặc sáng sớm)
  return 0.2f; // Sáng nhẹ để tiết kiệm điện và đảm bảo an ninh
}