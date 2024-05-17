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

  // Default route to serve index.html from /static/docs
  CROW_ROUTE(app, "/docs/")
  ([](crow::response &res) {
    res.redirect("/docs/index.html");
    res.end();
  });

  CROW_ROUTE(app, "/docs/<path>")
      .methods(crow::HTTPMethod::GET)(
          [](const crow::request &req, crow::response &res, std::string path) {
            std::string full_path = CROW_STATIC_DIRECTORY "docs/" + path;
            res.set_static_file_info(full_path);
            res.end();
          });

  CROW_ROUTE(app, "/")
  ([]() {
    auto page = crow::mustache::load("index.html");
    return page.render();
  });

  app.port(3000).multithreaded().run();
}
