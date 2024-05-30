#include "router.h"
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <crow/logging.h>

crow::response Router::handleGetStudents() {
  sqlite3_stmt *stmt;
  const char *sql =
      "SELECT students.id, students.first_name, students.surname, "
      "students.class, classes.name "
      "as 'class_name' FROM students JOIN classes ON students.class = "
      "classes.id";
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
    student["class_name"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    students.push_back(std::move(student));
  }
  sqlite3_finalize(stmt);

  crow::json::wvalue result;
  result = std::move(students);
  return crow::response(200, result);
}

crow::response Router::handleCreateStudent(const crow::json::rvalue &x) {
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

  int student_id = sqlite3_last_insert_rowid(this->db);
  sqlite3_finalize(stmt);

  crow::json::wvalue response;
  response["message"] = "Student created successfully";
  response["student_id"] = student_id;

  return crow::response(201, response);
}

crow::response Router::handleGetStudent(int id) {
  sqlite3_stmt *stmt;
  const char *sql =
      "SELECT students.id, students.first_name, students.surname, "
      "students.class, classes.name "
      "as 'class_name' FROM students JOIN classes ON students.class = "
      "classes.id WHERE students.id = ?";
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
    student["class_name"] =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    sqlite3_finalize(stmt);
    return crow::response(200, student);
  } else {
    sqlite3_finalize(stmt);
    return crow::response(404, "Student not found");
  }
}

crow::response Router::handleUpdateStudent(const crow::json::rvalue &data,
                                           int id) {
  std::string first_name = data["first_name"].s();
  std::string surname = data["surname"].s();
  int class_id = data["class"].i();

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

crow::response Router::handleDeleteStudent(int id) {
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
