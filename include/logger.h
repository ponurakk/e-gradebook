/** @file
 * @brief Defines a custom logger class for crow
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <crow/logging.h>
#include <string>

using crow::LogLevel, crow::ILogHandler;
using std::string;

/**
 * @class CustomLogger
 * @brief Custom logger class implementing the ILogHandler interface.
 */
class CustomLogger : public ILogHandler {
public:
  CustomLogger() {}

  /**
   * @brief Logs a message with the specified LogLevel.
   * @param message The message to be logged.
   * @param level The LogLevel of the message.
   */
  void log(string message, LogLevel level) override;
};

/**
 * @brief Converts a LogLevel enum value to its corresponding string representation.
 * @param level The LogLevel enum value.
 * @return The string representation of the log level.
 */
string logLevelToString(LogLevel level);

/**
 * @brief Converts a LogLevel enum value to its corresponding ascii color code.
 * @param level The LogLevel enum value.
 * @return The color code corresponding to the LogLevel.
 */
int logLevelToColor(LogLevel level);

/**
 * @brief Sets the terminal color using the provided color code.
 * @param color The color code to be set.
 */
void setColor(int color);

/**
 * @brief Resets the terminal color to default.
 */
void resetColor();

/**
 * @brief Gets the current time in the specified format.
 * @param format The format of the time string (e.g., "%Y-%m-%d %H:%M:%S").
 * @return The current time as a string formatted according to the provided format.
 */
string getCurrentTime(string format);

#endif
