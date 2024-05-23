/** @file
 * @brief Defines functions for database intialization
 */

#ifndef INITIALIZE_DB_H_
#define INITIALIZE_DB_H_

#include "config.h"
#include <sqlite3.h>

/**
 * @class DbInit
 * @brief Class responsible for initializing the database and creating tables.
 */
class DbInit {
private:
  sqlite3 *db; /**< Pointer to the SQLite database connection */

public:
  /**
   * @brief Constructor that initializes the database connection.
   * @param config Configuration object containing database settings.
   */
  DbInit(Config config);

  /**
   * @brief Destructor that closes the database connection.
   */
  ~DbInit();

  /**
   * @brief Returns sqlite3
   */
  sqlite3 *getDb();

private:
  /**
   * @brief Creates the Grades table in the database.
   */
  void createGradesTable();

  /**
   * @brief Creates the Teachers table in the database.
   */
  void createTeachersTable();

  /**
   * @brief Creates the Students table in the database.
   */
  void createStudentsTable();

  /**
   * @brief Creates the Classes table in the database.
   */
  void createClassesTable();
};

/**
 * @brief Helper function to handle SQLite results.
 * @param db Pointer to the SQLite database connection.
 * @param rc Result code from the SQLite operation.
 * @param successMessage Message to display on successful operation.
 * @param onSuccess Callback function to execute on successful operation.
 * @return Integer result code from the SQLite operation.
 */
int sqliteResult(sqlite3 *db, int rc, const char *successMessage,
                 std::function<void()> onSuccess);
#endif
