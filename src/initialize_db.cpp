#include "initialize_db.h"
#include <crow/logging.h>
#include <sqlite3.h>

int sqliteResult(sqlite3 *db, int rc, const char *successMessage,
                 std::function<void()> onSuccess) {
  if (rc == SQLITE_OK || rc == SQLITE_DONE || rc == SQLITE_ROW) {
    if (successMessage != nullptr) {
      CROW_LOG_INFO << successMessage;
    }
    if (onSuccess) {
      onSuccess();
    }
  } else {
    CROW_LOG_ERROR << "SQL error: " << sqlite3_errmsg(db);
  }
  return rc;
}

void DbInit::createClassesTable() {
  const char *createTable = "CREATE TABLE IF NOT EXISTS \"classes\" ("
                            "\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "\"name\" VARCHAR NOT NULL"
                            ");";

  int rc = sqlite3_exec(this->db, createTable, 0, 0, 0);

  sqliteResult(this->db, rc, "Succesfully created Classes table",
               [&] { this->createStudentsTable(); });
}

void DbInit::createStudentsTable() {
  const char *createTable =
      "CREATE TABLE IF NOT EXISTS \"students\" ("
      "\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,"
      "\"first_name\" VARCHAR NOT NULL,"
      "\"surname\" VARCHAR NOT NULL,"
      "\"class\" INTEGER,"
      "FOREIGN KEY(\"class\") REFERENCES \"classes\"(\"id\")"
      ");";

  int rc = sqlite3_exec(this->db, createTable, 0, 0, 0);

  sqliteResult(this->db, rc, "Succesfully created Students table",
               [&] { this->createTeachersTable(); });
}

void DbInit::createTeachersTable() {
  const char *createTable =
      "CREATE TABLE IF NOT EXISTS \"teachers\" ("
      "\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,"
      "\"first_name\" VARCHAR NOT NULL,"
      "\"surname\" VARCHAR NOT NULL,"
      "\"subject\" VARCHAR NOT NULL,"
      "\"teacher_of\" INTEGER,"
      "FOREIGN KEY(\"teacher_of\") REFERENCES \"classes\"(\"id\")"
      ");";

  int rc = sqlite3_exec(this->db, createTable, 0, 0, 0);

  sqliteResult(this->db, rc, "Succesfully created Teachers table",
               [&] { this->createGradesTable(); });
}

void DbInit::createGradesTable() {
  const char *createTable =
      "CREATE TABLE IF NOT EXISTS \"grades\" ("
      "\"student_id\" INTEGER,"
      "\"teacher_id\" INTEGER,"
      "\"score\" INTEGER NOT NULL,"
      "\"created_at\" INTEGER NOT NULL,"
      "FOREIGN KEY(\"student_id\") REFERENCES \"students\"(\"id\"),"
      "FOREIGN KEY(\"teacher_id\") REFERENCES \"teachers\"(\"id\")"
      ");";

  int rc = sqlite3_exec(this->db, createTable, 0, 0, 0);

  sqliteResult(this->db, rc, "Succesfully created Grades table", nullptr);
}

DbInit::DbInit(Config config) {
  int rc = sqlite3_open(config.database.c_str(), &this->db);

  sqliteResult(this->db, rc, "Opened database successfully",
               [&] { this->createClassesTable(); });

  sqlite3_close(this->db);
}

sqlite3 *DbInit::getDb() { return this->db; }

DbInit::~DbInit() {
  sqlite3_close(this->db);
  CROW_LOG_DEBUG << "Closed db connection";
}
