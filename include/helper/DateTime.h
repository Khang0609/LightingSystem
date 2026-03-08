#pragma once
#include <string>

/**
 * @brief Utility to get the current system time in decimal format (e.g., 14.5
 * for 14:30)
 */
double getCurrentTimeInDecimal();

/**
 * @brief Utility to get date
 *
 */
void getCurrentDate(int &day, int &month, int &year);

/**
 * @brief Utility to format decimal time to string and print it
 */
void printFormattedTime(const std::string &label, double decimalTime);