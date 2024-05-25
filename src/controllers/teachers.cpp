#include "router.h"
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/logging.h>

crow::response Router::handleGetTeachers(const crow::request &req) {
  sqlite3_stmt *stmt;
  const char *sql =
      "SELECT teachers.id, teachers.first_name, teachers.surname, "
      "teachers.subject, classes.name as 'teacher_of' FROM teachers JOIN "
      "classes ON classes.id = teachers.teacher_of";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }

  std::vector<crow::json::wvalue> teachers;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue teacher;
    teacher["id"] = sqlite3_column_int(stmt, 0);
    teacher["first_name"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    teacher["surname"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    teacher["subject"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    teacher["teacher_of"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    teachers.push_back(std::move(teacher));
  }
  sqlite3_finalize(stmt);

  crow::json::wvalue result;
  result = std::move(teachers);
  return crow::response(200, result);
}

crow::response Router::handleCreateTeacher(const crow::request &req) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  std::string first_name = x["first_name"].s();
  std::string surname = x["surname"].s();
  std::string subject = x["subject"].s();
  int teacher_of = x["teacher_of"].i();

  const char *sql = "INSERT INTO teachers (first_name, surname, subject, "
                    "teacher_of) VALUES (?, ?, ?, ?)";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_text(stmt, 1, first_name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, subject.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 4, teacher_of);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }

  int teacher_id = sqlite3_last_insert_rowid(this->db);
  sqlite3_finalize(stmt);

  crow::json::wvalue response;
  response["message"] = "Teacher created successfully";
  response["teacher_id"] = teacher_id;

  return crow::response(201, response);
}

crow::response Router::handleGetTeacher(const crow::request &req, int id) {
  sqlite3_stmt *stmt;
  const char *sql =
      "SELECT teachers.id, teachers.first_name, teachers.surname, "
      "teachers.subject, classes.name as 'teacher_of' FROM teachers JOIN "
      "classes ON classes.id = teachers.teacher_of WHERE teachers.id = ?";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_int(stmt, 1, id);

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue teacher;
    teacher["id"] = sqlite3_column_int(stmt, 0);
    teacher["first_name"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    teacher["surname"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    teacher["subject"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    teacher["teacher_of"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    sqlite3_finalize(stmt);
    return crow::response(200, teacher);
  } else {
    sqlite3_finalize(stmt);
    return crow::response(404, "Teacher not found");
  }
}

crow::response Router::handleUpdateTeacher(const crow::request &req, int id) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  std::string first_name = x["first_name"].s();
  std::string surname = x["surname"].s();
  std::string subject = x["subject"].s();
  int teacher_of = x["teacher_of"].i();

  const char *sql = "UPDATE teachers SET first_name = ?, surname = ?, "
                    "subject = ?, teacher_of = ? WHERE id = ?";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_text(stmt, 1, first_name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, surname.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, subject.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 4, teacher_of);
  sqlite3_bind_int(stmt, 5, id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(200, "Teacher updated successfully");
}

crow::response Router::handleDeleteTeacher(const crow::request &req, int id) {
  const char *sql = "DELETE FROM teachers WHERE id = ?";
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

  return crow::response(200, "Teacher deleted successfully");
}
