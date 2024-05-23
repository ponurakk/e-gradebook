#include "router.h"
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/logging.h>

crow::response Router::handleGetGrades(const crow::request &req) {
  sqlite3_stmt *stmt;
  const char *sql = "SELECT * FROM grades";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }

  std::vector<crow::json::wvalue> grades;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue grade;
    grade["id"] = sqlite3_column_int(stmt, 0);
    grade["student_id"] = sqlite3_column_int(stmt, 1);
    grade["teacher_id"] = sqlite3_column_int(stmt, 2);
    grade["score"] = sqlite3_column_int(stmt, 3);
    grade["created_at"] = sqlite3_column_int(stmt, 4);
    grades.push_back(std::move(grade));
  }
  sqlite3_finalize(stmt);

  crow::json::wvalue result;
  result["grades"] = std::move(grades);
  return crow::response(200, result);
}

crow::response Router::handleCreateGrade(const crow::request &req) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  int student_id = x["student_id"].i();
  int teacher_id = x["teacher_id"].i();
  int score = x["score"].i();
  int created_at = x["created_at"].i();

  const char *sql = "INSERT INTO grades (student_id, teacher_id, score, "
                    "created_at) VALUES (?, ?, ?, ?)";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_int(stmt, 1, student_id);
  sqlite3_bind_int(stmt, 2, teacher_id);
  sqlite3_bind_int(stmt, 3, score);
  sqlite3_bind_int(stmt, 4, created_at);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(201, "Grade created successfully");
}

crow::response Router::handleGetGrade(const crow::request &req, int id) {
  sqlite3_stmt *stmt;
  const char *sql = "SELECT * FROM grades WHERE id = ?";
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_int(stmt, 1, id);

  if (sqlite3_step(stmt) == SQLITE_ROW) {
    crow::json::wvalue grade;
    grade["id"] = sqlite3_column_int(stmt, 0);
    grade["student_id"] = sqlite3_column_int(stmt, 1);
    grade["teacher_id"] = sqlite3_column_int(stmt, 2);
    grade["score"] = sqlite3_column_int(stmt, 3);
    grade["created_at"] = sqlite3_column_int(stmt, 4);
    sqlite3_finalize(stmt);
    return crow::response(200, grade);
  } else {
    sqlite3_finalize(stmt);
    return crow::response(404, "Grade not found");
  }
}

crow::response Router::handleUpdateGrade(const crow::request &req, int id) {
  auto x = crow::json::load(req.body);
  if (!x) {
    return crow::response(400, "Invalid JSON");
  }

  int student_id = x["student_id"].i();
  int teacher_id = x["teacher_id"].i();
  int score = x["score"].i();
  int created_at = x["created_at"].i();

  const char *sql = "UPDATE grades SET student_id = ?, teacher_id = ?, score "
                    "= ?, created_at = ? WHERE id = ?";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(this->db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    return crow::response(500, "Failed to prepare statement");
  }
  sqlite3_bind_int(stmt, 1, student_id);
  sqlite3_bind_int(stmt, 2, teacher_id);
  sqlite3_bind_int(stmt, 3, score);
  sqlite3_bind_int(stmt, 4, created_at);
  sqlite3_bind_int(stmt, 5, id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    return crow::response(500, "Failed to execute statement");
  }
  sqlite3_finalize(stmt);

  return crow::response(200, "Grade updated successfully");
}

crow::response Router::handleDeleteGrade(const crow::request &req, int id) {
  const char *sql = "DELETE FROM grades WHERE id = ?";
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

  return crow::response(200, "Grade deleted successfully");
}
