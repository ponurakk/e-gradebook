#ifndef CONFIG_H_
#define CONFIG_H_

#define DATABASE_NAME "sqlite.db"

#include <crow/json.h>

using std::string;

struct Config {
  string addr;
  int port;
  string database;

  operator crow::json::wvalue() const;
};

Config get_config();

#endif
