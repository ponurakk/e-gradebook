cmake_minimum_required(VERSION 3.12)
project(e-gradebook VERSION 0.1.0)

# Find packages
find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)

# Add files to package
set(SOURCE_FILES
  src/main.cpp
  src/logger.cpp
  src/initialize_db.cpp
  src/controllers/router.cpp
  src/controllers/classes.cpp
  src/controllers/grades.cpp
  src/controllers/students.cpp
  src/controllers/teachers.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link against the libraries
target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow SQLite::SQLite3)
