#include "router.h"
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <crow/logging.h>
#include <crow/websocket.h>
#include <optional>
#include <string>

using crow::json::rvalue;
using crow::json::wvalue;
using crow::websocket::connection;
using std::nullopt;

std::string_view getLastElement(const std::string_view &str, char delim) {
  size_t pos = str.find_last_of(delim);
  if (pos != std::string_view::npos) {
    return str.substr(pos + 1);
  }
  return str;
}

std::optional<rvalue> validateJSON(std::string data) {
  auto x = crow::json::load(data);
  if (!x) {
    return nullopt;
  }
  CROW_LOG_INFO << x;
  return x;
}

void Router::handleWebsocketRoute(connection &conn, std::string method,
                                  std::string url, rvalue data) {
  if (method == "GET") {
    std::string id_str = std::string(getLastElement(url, '/'));
    int id = std::stoi(id_str);
    if (url == "/api/students") {
      crow::response res = this->handleGetStudents();
      conn.send_text(res.body);
    } else if (url == "/api/classes") {
      crow::response res = this->handleGetClasses();
      conn.send_text(res.body);
    } else if (url == "/api/teachers") {
      crow::response res = this->handleGetTeachers();
      conn.send_text(res.body);
    } else if (url == "/api/grades") {
      crow::response res = this->handleGetGrades();
      conn.send_text(res.body);
    } else if (url == "/api/students/" + id_str) {
      crow::response res = this->handleGetStudent(id);
      conn.send_text(res.body);
    } else if (url == "/api/classes/" + id_str) {
      crow::response res = this->handleGetClass(id);
      conn.send_text(res.body);
    } else if (url == "/api/teachers/" + id_str) {
      crow::response res = this->handleGetTeacher(id);
      conn.send_text(res.body);
    } else if (url == "/api/grades/" + id_str) {
      crow::response res = this->handleGetGrade(id);
      conn.send_text(res.body);
    }
  } else if (method == "POST") {
    if (url == "/api/students") {
      crow::response res = this->handleCreateStudent(data);
      conn.send_text(res.body);
    } else if (url == "/api/classes") {
      crow::response res = this->handleCreateClass(data);
      conn.send_text(res.body);
    } else if (url == "/api/teachers") {
      crow::response res = this->handleCreateTeacher(data);
      conn.send_text(res.body);
    } else if (url == "/api/grades") {
      crow::response res = this->handleCreateGrade(data);
      conn.send_text(res.body);
    }
  } else if (method == "PUT") {
    std::string id_str = std::string(getLastElement(url, '/'));
    int id = std::stoi(id_str);
    if (url == "/api/students" + id_str) {
      crow::response res = this->handleCreateStudent(data);
      conn.send_text(res.body);
    } else if (url == "/api/classes" + id_str) {
      crow::response res = this->handleUpdateClass(data, id);
      conn.send_text(res.body);
    } else if (url == "/api/teachers" + id_str) {
      crow::response res = this->handleUpdateTeacher(data, id);
      conn.send_text(res.body);
    } else if (url == "/api/grades" + id_str) {
      crow::response res = this->handleUpdateGrade(data, id);
      conn.send_text(res.body);
    }
  } else if (method == "DELETE") {
    std::string id_str = std::string(getLastElement(url, '/'));
    int id = std::stoi(id_str);
    if (url == "/api/students" + id_str) {
      crow::response res = this->handleDeleteStudent(id);
      conn.send_text(res.body);
    } else if (url == "/api/classes" + id_str) {
      crow::response res = this->handleDeleteClass(id);
      conn.send_text(res.body);
    } else if (url == "/api/teachers" + id_str) {
      crow::response res = this->handleDeleteTeacher(id);
      conn.send_text(res.body);
    } else if (url == "/api/grades" + id_str) {
      crow::response res = this->handleDeleteGrade(id);
      conn.send_text(res.body);
    }
  }
}

void Router::onWebsocketMessage(crow::websocket::connection &conn,
                                const std::string &data) {
  rvalue x = crow::json::load(data);
  wvalue res;
  if (!x) {
    res["code"] = 400;
    res["message"] = "Invalid JSON";
    conn.send_text(res.dump());
  } else {
    try {
      std::string method = x["method"].s();
      std::string url = x["url"].s();
      rvalue data = x["data"];

      handleWebsocketRoute(conn, method, url, data);
    } catch (...) {
      res["code"] = 400;
      res["message"] = "Invalid JSON";
      conn.send_text(res.dump());
    }
  }
}

Router::Router(crow::SimpleApp &app) : app(app) {
  int rc = sqlite3_open("sqlite.db", &this->db);
  if (rc != SQLITE_OK) {
    CROW_LOG_ERROR << "Failed to connect to db";
    return;
  }

  // Define routes for students
  CROW_ROUTE(app, "/api/students")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetStudents(); });
  CROW_ROUTE(app, "/api/students")
      .methods(crow::HTTPMethod::POST)([&](const crow::request &req) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleCreateStudent(*data);
      });
  CROW_ROUTE(app, "/api/students/<int>")
      .methods(crow::HTTPMethod::GET)([&](const crow::request &req, int id) {
        return handleGetStudent(id);
      });
  CROW_ROUTE(app, "/api/students/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleUpdateStudent(*data, id);
      });
  CROW_ROUTE(app, "/api/students/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteStudent(id);
      });

  // Define routes for classes
  CROW_ROUTE(app, "/api/classes")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetClasses(); });
  CROW_ROUTE(app, "/api/classes")
      .methods(crow::HTTPMethod::POST)([&](const crow::request &req) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleCreateClass(*data);
      });
  CROW_ROUTE(app, "/api/classes/<int>")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req, int id) { return handleGetClass(id); });
  CROW_ROUTE(app, "/api/classes/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleUpdateClass(*data, id);
      });
  CROW_ROUTE(app, "/api/classes/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteClass(id);
      });

  // Define routes for grades
  CROW_ROUTE(app, "/api/grades")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetGrades(); });
  CROW_ROUTE(app, "/api/grades")
      .methods(crow::HTTPMethod::POST)([&](const crow::request &req) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleCreateGrade(*data);
      });
  CROW_ROUTE(app, "/api/grades/<int>")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req, int id) { return handleGetGrade(id); });
  CROW_ROUTE(app, "/api/grades/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleUpdateGrade(*data, id);
      });
  CROW_ROUTE(app, "/api/grades/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteGrade(id);
      });

  // Define routes for teachers
  CROW_ROUTE(app, "/api/teachers")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetTeachers(); });
  CROW_ROUTE(app, "/api/teachers")
      .methods(crow::HTTPMethod::POST)([&](const crow::request &req) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleCreateTeacher(*data);
      });
  CROW_ROUTE(app, "/api/teachers/<int>")
      .methods(crow::HTTPMethod::GET)([&](const crow::request &req, int id) {
        return handleGetTeacher(id);
      });
  CROW_ROUTE(app, "/api/teachers/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        auto data = validateJSON(req.body);
        if (!data) {
          wvalue res;
          res["message"] = "Invalid JSON";
          return crow::response(400, res);
        }
        return handleUpdateTeacher(*data, id);
      });
  CROW_ROUTE(app, "/api/teachers/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteTeacher(id);
      });

  // Default route to serve index.html from /static/docs
  CROW_ROUTE(app, "/docs/")
  ([](crow::response &res) {
    res.redirect("/docs/index.html");
    res.end();
  });

  CROW_ROUTE(app, "/docs/<path>")
      .methods(crow::HTTPMethod::GET)(
          [](const crow::request &req, crow::response &res, std::string path) {
            std::string full_path = "docs/" + path;
            res.set_static_file_info(full_path);
            res.end();
          });

  CROW_ROUTE(app, "/")
  ([]() {
    auto page = crow::mustache::load("index.mustache");
    return page.render();
  });

  CROW_ROUTE(app, "/ws")
      .websocket()
      .onopen([](crow::websocket::connection &conn) {
        // Handle WebSocket connection open event
        CROW_LOG_INFO << "WebSocket connection opened";
      })
      .onmessage(
          [this](crow::websocket::connection &conn, const std::string &data,
                 bool is_binary) { this->onWebsocketMessage(conn, data); })
      .onclose(
          [](crow::websocket::connection &conn, const std::string &reason) {
            // Handle WebSocket connection close event
            CROW_LOG_INFO << "WebSocket connection closed: " << reason;
          });
}

Router::~Router() {
  if (this->db) {
    sqlite3_close(this->db);
  }
}
