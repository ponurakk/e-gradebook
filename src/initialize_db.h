#ifndef INITIALIZE_DB_H_
#define INITIALIZE_DB_H_

#include "config.h"
#include <sqlite3.h>

class DbInit {
private:
  sqlite3 *db;

public:
  DbInit(Config config);
  ~DbInit();

private:
  void createGradesTable();
  void createTeachersTable();
  void createStudentsTable();
  void createClassesTable();
};

int sqliteResult(sqlite3 *db, int rc, const char *successMessage,
                 std::function<void()> onSuccess);
#endif
