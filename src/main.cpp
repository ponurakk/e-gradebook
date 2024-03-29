#include "logger.h"
#include <crow/app.h>
#include <crow/logging.h>

using crow::LogLevel;
using std::string;

int main() {
  CustomLogger logger;
  crow::logger::setHandler(&logger);
  crow::SimpleApp app;
  app.loglevel(LogLevel::INFO);

  CROW_ROUTE(app, "/")
  ([]() {
    auto page = crow::mustache::load("index.html");
    return page.render();
  });

  app.port(3000).multithreaded().run();
}
