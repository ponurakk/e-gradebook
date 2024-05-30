#ifndef ROUTER_H_
#define ROUTER_H_

#include <crow/app.h>
#include <crow/json.h>
#include <crow/websocket.h>
#include <sqlite3.h>

/**
 * @class Router
 * @brief Handles routing for a web application using the Crow framework and
 * SQLite database.
 */
class Router {
private:
  crow::SimpleApp &app; ///< Reference to the Crow application.
  sqlite3 *db;          ///< Pointer to the SQLite database connection.

public:
  /**
   * @brief Constructor for Router.
   * @param app Reference to the Crow application.
   */
  Router(crow::SimpleApp &app);

  /**
   * @brief Destructor for Router.
   */
  ~Router();

private:
  /**
   * @brief Handles WebSocket route requests.
   *
   * This function is responsible for handling WebSocket route requests. It
   * receives WebSocket connections, along with method, URL, and data, and
   * processes them accordingly.
   *
   * @param conn Reference to the WebSocket connection.
   * @param method The HTTP method used for the request.
   * @param url The URL of the WebSocket route.
   * @param data The JSON data associated with the WebSocket request.
   */
  void handleWebsocketRoute(crow::websocket::connection &conn,
                            std::string method, std::string url,
                            crow::json::rvalue data);

  /**
   * @brief Handles incoming WebSocket messages.
   *
   * This function is called when a WebSocket message is received. It processes
   * the incoming message and performs any necessary actions.
   *
   * @param conn Reference to the WebSocket connection.
   * @param data The content of the WebSocket message.
   */
  void onWebsocketMessage(crow::websocket::connection &conn,
                          const std::string &data);

  // CRUD operations for students
  /**
   * @brief Handles GET requests to retrieve all students.
   * @return The Crow response object containing the list of students.
   */
  crow::response handleGetStudents();
  /**
   * @brief Handles POST requests to create a new student.
   * @param data The JSON data containing the student information.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateStudent(const crow::json::rvalue &data);
  /**
   * @brief Handles GET requests to retrieve a specific student by ID.
   * @param id The ID of the student to retrieve.
   * @return The Crow response object containing the student data.
   */
  crow::response handleGetStudent(int id);
  /**
   * @brief Handles PUT requests to update a specific student by ID.
   * @param data The JSON data containing the updated student information.
   * @param id The ID of the student to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateStudent(const crow::json::rvalue &data, int id);
  /**
   * @brief Handles DELETE requests to delete a specific student by ID.
   * @param id The ID of the student to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteStudent(int id);

  // CRUD operations for classes
  /**
   * @brief Handles GET requests to retrieve all classes.
   * @return The Crow response object containing the list of classes.
   */
  crow::response handleGetClasses();
  /**
   * @brief Handles POST requests to create a new class.
   * @param data The JSON data containing the class information.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateClass(const crow::json::rvalue &data);
  /**
   * @brief Handles GET requests to retrieve a specific class by ID.
   * @param id The ID of the class to retrieve.
   * @return The Crow response object containing the class data.
   */
  crow::response handleGetClass(int id);
  /**
   * @brief Handles PUT requests to update a specific class by ID.
   * @param data The JSON data containing the updated class information.
   * @param id The ID of the class to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateClass(const crow::json::rvalue &data, int id);
  /**
   * @brief Handles DELETE requests to delete a specific class by ID.
   * @param id The ID of the class to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteClass(int id);

  // CRUD operations for grades
  /**
   * @brief Handles GET requests to retrieve all grades.
   * @return The Crow response object containing the list of grades.
   */
  crow::response handleGetGrades();
  /**
   * @brief Handles POST requests to create a new grade.
   * @param data The JSON data containing the grade information.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateGrade(const crow::json::rvalue &data);
  /**
   * @brief Handles GET requests to retrieve a specific grade by ID.
   * @param id The ID of the grade to retrieve.
   * @return The Crow response object containing the grade data.
   */
  crow::response handleGetGrade(int id);
  /**
   * @brief Handles PUT requests to update a specific grade by ID.
   * @param data The JSON data containing the updated grade information.
   * @param id The ID of the grade to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateGrade(const crow::json::rvalue &data, int id);
  /**
   * @brief Handles DELETE requests to delete a specific grade by ID.
   * @param id The ID of the grade to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteGrade(int id);

  // CRUD operations for teachers
  /**
   * @brief Handles GET requests to retrieve all teachers.
   * @return The Crow response object containing the list of teachers.
   */
  crow::response handleGetTeachers();
  /**
   * @brief Handles POST requests to create a new teacher.
   * @param data The JSON data containing the teacher information.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateTeacher(const crow::json::rvalue &data);
  /**
   * @brief Handles GET requests to retrieve a specific teacher by ID.
   * @param id The ID of the teacher to retrieve.
   * @return The Crow response object containing the teacher data.
   */
  crow::response handleGetTeacher(int id);
  /**
   * @brief Handles PUT requests to update a specific teacher by ID.
   * @param data The JSON data containing the updated teacher information.
   * @param id The ID of the teacher to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateTeacher(const crow::json::rvalue &data, int id);
  /**
   * @brief Handles DELETE requests to delete a specific teacher by ID.
   * @param id The ID of the teacher to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteTeacher(int id);
};

#endif
