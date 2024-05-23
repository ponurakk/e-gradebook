#include "router.h"
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/logging.h>

crow::response Router::handleGetStudents(const crow::request &req) {
  sqlite3_stmt *stmt;
  const char *sql = "SELECT * FROM students";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }

  std::vector<crow::json::wvalue> students;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue student;
    student["id"] = sqlite3_column_int(stmt, 0);
    student["first_name"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    student["surname"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    student["class"] = sqlite3_column_int(stmt, 3);
    students.push_back(std::move(student));
  }
  sqlite3_finalize(stmt);

  crow::json::wvalue result;
  result["students"] = std::move(students);
  return crow::response(200, result);
}

crow::response Router::handleCreateStudent(const crow::request &req) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  std::string first_name = x["first_name"].s();
  std::string surname = x["surname"].s();
  int class_id = x["class"].i();

  const char *sql =
      "INSERT INTO students (first_name, surname, class) VALUES (?, ?, ?)";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_text(stmt, 1, first_name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, class_id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(201, "Student created successfully");
}

crow::response Router::handleGetStudent(const crow::request &req, int id) {
  sqlite3_stmt *stmt;
  const char *sql = "SELECT * FROM students WHERE id = ?";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_int(stmt, 1, id);

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue student;
    student["id"] = sqlite3_column_int(stmt, 0);
    student["first_name"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    student["surname"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    student["class"] = sqlite3_column_int(stmt, 3);
    sqlite3_finalize(stmt);
    return crow::response(200, student);
  } else {
    sqlite3_finalize(stmt);
    return crow::response(404, "Student not found");
  }
}

crow::response Router::handleUpdateStudent(const crow::request &req, int id) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  std::string first_name = x["first_name"].s();
  std::string surname = x["surname"].s();
  int class_id = x["class"].i();

  const char *sql = "UPDATE students SET first_name = ?, surname = ?, class "
                    "= ? WHERE id = ?";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_text(stmt, 1, first_name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, class_id);
  sqlite3_bind_int(stmt, 4, id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(200, "Student updated successfully");
}

crow::response Router::handleDeleteStudent(const crow::request &req, int id) {
  const char *sql = "DELETE FROM students WHERE id = ?";
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

  return crow::response(200, "Student deleted successfully");
}
