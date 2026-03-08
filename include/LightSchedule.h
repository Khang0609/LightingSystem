#pragma once

constexpr double PI = 3.14159265358979323846;

/**
 * @brief Properties for sun calculation
 */
struct SunCalculationProps {
  double longitude; // in radians
  double latitude;  // in radians
  int dayOfYear;
  int timeZone;
};

/**
 * @brief Utility to get day index from date
 */
int getDayOfYear(int day, int month, int year);

/**
 * @brief Calculate sunrise time (dawn)
 */
double calculateDawnTime(const SunCalculationProps &p);

/**
 * @brief Calculate sunset time (twilight)
 */
double calculateTwilightTime(const SunCalculationProps &p);

/**
 * @brief Calculate the light level based on schedule (Peak hours vs Normal
 * night)
 */
float getScheduleLevel(double currentTime, double dawnTime, double sunsetTime);
