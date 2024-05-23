#include "router.h"
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/logging.h>

crow::response Router::handleGetClasses(const crow::request &req) {
  sqlite3_stmt *stmt;
  const char *sql = "SELECT * FROM classes";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }

  std::vector<crow::json::wvalue> classes;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue cls;
    cls["id"] = sqlite3_column_int(stmt, 0);
    cls["name"] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    classes.push_back(std::move(cls));
  }
  sqlite3_finalize(stmt);

  crow::json::wvalue result;
  result["classes"] = std::move(classes);
  return crow::response(200, result);
}

crow::response Router::handleCreateClass(const crow::request &req) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  std::string name = x["name"].s();

  const char *sql = "INSERT INTO classes(name) VALUES (?)";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(201, "Class created successfully");
}

crow::response Router::handleGetClass(const crow::request &req, int id) {
  sqlite3_stmt *stmt;
  const char *sql = "SELECT * FROM classes WHERE id = ?";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_int(stmt, 1, id);

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue cls;
    cls["id"] = sqlite3_column_int(stmt, 0);
    cls["name"] = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    sqlite3_finalize(stmt);
    return crow::response(200, cls);
  } else {
    sqlite3_finalize(stmt);
    return crow::response(404, "Class not found");
  }
}

crow::response Router::handleUpdateClass(const crow::request &req, int id) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  std::string name = x["name"].s();

  const char *sql = "UPDATE classes SET name = ? WHERE id = ?";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 2, id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(200, "Class updated successfully");
}

crow::response Router::handleDeleteClass(const crow::request &req, int id) {
  const char *sql = "DELETE FROM classes WHERE id = ?";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_int(stmt, 1, id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(200, "Class deleted successfully");
}
