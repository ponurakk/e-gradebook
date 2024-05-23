#ifndef ROUTER_H_
#define ROUTER_H_

#include <crow/app.h>
#include <sqlite3.h>

/**
 * @class Router
 * @brief A class that handles routing for a web application using the Crow
 * framework and SQLite database.
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
   * @brief Handles GET requests to retrieve all students.
   * @param req The Crow request object.
   * @return The Crow response object containing the list of students.
   */
  crow::response handleGetStudents(const crow::request &req);

  /**
   * @brief Handles POST requests to create a new student.
   * @param req The Crow request object containing the student data.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateStudent(const crow::request &req);

  /**
   * @brief Handles GET requests to retrieve a specific student by ID.
   * @param req The Crow request object.
   * @param id The ID of the student to retrieve.
   * @return The Crow response object containing the student data.
   */
  crow::response handleGetStudent(const crow::request &req, int id);

  /**
   * @brief Handles PUT requests to update a specific student by ID.
   * @param req The Crow request object containing the updated student data.
   * @param id The ID of the student to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateStudent(const crow::request &req, int id);

  /**
   * @brief Handles DELETE requests to delete a specific student by ID.
   * @param req The Crow request object.
   * @param id The ID of the student to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteStudent(const crow::request &req, int id);

  /**
   * @brief Handles GET requests to retrieve all classes.
   * @param req The Crow request object.
   * @return The Crow response object containing the list of classes.
   */
  crow::response handleGetClasses(const crow::request &req);

  /**
   * @brief Handles POST requests to create a new class.
   * @param req The Crow request object containing the class data.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateClass(const crow::request &req);

  /**
   * @brief Handles GET requests to retrieve a specific class by ID.
   * @param req The Crow request object.
   * @param id The ID of the class to retrieve.
   * @return The Crow response object containing the class data.
   */
  crow::response handleGetClass(const crow::request &req, int id);

  /**
   * @brief Handles PUT requests to update a specific class by ID.
   * @param req The Crow request object containing the updated class data.
   * @param id The ID of the class to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateClass(const crow::request &req, int id);

  /**
   * @brief Handles DELETE requests to delete a specific class by ID.
   * @param req The Crow request object.
   * @param id The ID of the class to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteClass(const crow::request &req, int id);

  /**
   * @brief Handles GET requests to retrieve all grades.
   * @param req The Crow request object.
   * @return The Crow response object containing the list of grades.
   */
  crow::response handleGetGrades(const crow::request &req);

  /**
   * @brief Handles POST requests to create a new grade.
   * @param req The Crow request object containing the grade data.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateGrade(const crow::request &req);

  /**
   * @brief Handles GET requests to retrieve a specific grade by ID.
   * @param req The Crow request object.
   * @param id The ID of the grade to retrieve.
   * @return The Crow response object containing the grade data.
   */
  crow::response handleGetGrade(const crow::request &req, int id);

  /**
   * @brief Handles PUT requests to update a specific grade by ID.
   * @param req The Crow request object containing the updated grade data.
   * @param id The ID of the grade to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateGrade(const crow::request &req, int id);

  /**
   * @brief Handles DELETE requests to delete a specific grade by ID.
   * @param req The Crow request object.
   * @param id The ID of the grade to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteGrade(const crow::request &req, int id);

  /**
   * @brief Handles GET requests to retrieve all teachers.
   * @param req The Crow request object.
   * @return The Crow response object containing the list of teachers.
   */
  crow::response handleGetTeachers(const crow::request &req);

  /**
   * @brief Handles POST requests to create a new teacher.
   * @param req The Crow request object containing the teacher data.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleCreateTeacher(const crow::request &req);

  /**
   * @brief Handles GET requests to retrieve a specific teacher by ID.
   * @param req The Crow request object.
   * @param id The ID of the teacher to retrieve.
   * @return The Crow response object containing the teacher data.
   */
  crow::response handleGetTeacher(const crow::request &req, int id);

  /**
   * @brief Handles PUT requests to update a specific teacher by ID.
   * @param req The Crow request object containing the updated teacher data.
   * @param id The ID of the teacher to update.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleUpdateTeacher(const crow::request &req, int id);

  /**
   * @brief Handles DELETE requests to delete a specific teacher by ID.
   * @param req The Crow request object.
   * @param id The ID of the teacher to delete.
   * @return The Crow response object indicating the result of the operation.
   */
  crow::response handleDeleteTeacher(const crow::request &req, int id);
};

#endif
