#include "config.h"
#include "controllers/router.h"
#include "initialize_db.h"
#include "logger.h"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/logging.h>

using crow::LogLevel;
using std::string;

int main() {
  CustomLogger logger;
  crow::logger::setHandler(&logger);
  crow::SimpleApp app;
  app.loglevel(LogLevel::INFO);

  Config config = {"127.0.0.1", 3000, "sqlite.db"};
  DbInit db(config);
  Router router(app);

  app.port(3000).multithreaded().run();
}
