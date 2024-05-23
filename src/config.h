/** @file
 * @brief Defines Configuration
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define DATABASE_NAME "sqlite.db"

#include <crow/json.h>

using std::string;

/**
 * @struct Config
 * @brief Struct to hold configuration settings for the database.
 */
struct Config {
  std::string addr;     /**< Address of the database server */
  int port;             /**< Port number for the database server */
  std::string database; /**< Name of the database */

  /**
   * @brief Converts the Config object to a crow::json::wvalue object.
   * @return crow::json::wvalue JSON representation of the Config object.
   */
  operator crow::json::wvalue() const;
};

Config get_config();

#endif
