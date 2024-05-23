#include "router.h"
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/logging.h>

Router::Router(crow::SimpleApp &app) : app(app) {
  int rc = sqlite3_open("sqlite.db", &this->db);
  if (rc != SQLITE_OK) {
    CROW_LOG_ERROR << "Failed to connect to db";
    return;
  }

  // Define routes for students
  CROW_ROUTE(app, "/students")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetStudents(req); });
  CROW_ROUTE(app, "/students")
      .methods(crow::HTTPMethod::POST)(
          [&](const crow::request &req) { return handleCreateStudent(req); });
  CROW_ROUTE(app, "/students/<int>")
      .methods(crow::HTTPMethod::GET)([&](const crow::request &req, int id) {
        return handleGetStudent(req, id);
      });
  CROW_ROUTE(app, "/students/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        return handleUpdateStudent(req, id);
      });
  CROW_ROUTE(app, "/students/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteStudent(req, id);
      });

  // Define routes for classes
  CROW_ROUTE(app, "/classes")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetClasses(req); });
  CROW_ROUTE(app, "/classes")
      .methods(crow::HTTPMethod::POST)(
          [&](const crow::request &req) { return handleCreateClass(req); });
  CROW_ROUTE(app, "/classes/<int>")
      .methods(crow::HTTPMethod::GET)([&](const crow::request &req, int id) {
        return handleGetClass(req, id);
      });
  CROW_ROUTE(app, "/classes/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        return handleUpdateClass(req, id);
      });
  CROW_ROUTE(app, "/classes/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteClass(req, id);
      });

  // Define routes for grades
  CROW_ROUTE(app, "/grades")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetGrades(req); });
  CROW_ROUTE(app, "/grades")
      .methods(crow::HTTPMethod::POST)(
          [&](const crow::request &req) { return handleCreateGrade(req); });
  CROW_ROUTE(app, "/grades/<int>")
      .methods(crow::HTTPMethod::GET)([&](const crow::request &req, int id) {
        return handleGetGrade(req, id);
      });
  CROW_ROUTE(app, "/grades/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        return handleUpdateGrade(req, id);
      });
  CROW_ROUTE(app, "/grades/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteGrade(req, id);
      });

  // Define routes for teachers
  CROW_ROUTE(app, "/teachers")
      .methods(crow::HTTPMethod::GET)(
          [&](const crow::request &req) { return handleGetTeachers(req); });
  CROW_ROUTE(app, "/teachers")
      .methods(crow::HTTPMethod::POST)(
          [&](const crow::request &req) { return handleCreateTeacher(req); });
  CROW_ROUTE(app, "/teachers/<int>")
      .methods(crow::HTTPMethod::GET)([&](const crow::request &req, int id) {
        return handleGetTeacher(req, id);
      });
  CROW_ROUTE(app, "/teachers/<int>")
      .methods(crow::HTTPMethod::PUT)([&](const crow::request &req, int id) {
        return handleUpdateTeacher(req, id);
      });
  CROW_ROUTE(app, "/teachers/<int>")
      .methods(crow::HTTPMethod::DELETE)([&](const crow::request &req, int id) {
        return handleDeleteTeacher(req, id);
      });
}

Router::~Router() {
  if (this->db) {
    sqlite3_close(this->db);
  }
}
