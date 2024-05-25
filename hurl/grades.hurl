GET http://localhost:3000/api/grades
HTTP 200
[Asserts]
jsonpath "$[0].created_at" == 2024
jsonpath "$[0].subject" == "Polski"
jsonpath "$[0].teacher_surname" == "Tomaszewska"
jsonpath "$[0].teacher_first_name" == "Aurelia"
jsonpath "$[0].student_surname" == "Borowska"
jsonpath "$[0].student_first_name" == "Klara"
jsonpath "$[0].score" == 4
jsonpath "$[0].id" == 1


POST http://localhost:3000/api/grades
{
  "student_id": 2,
  "teacher_id": 1,
  "score": 4,
  "created_at": 2024
}
HTTP 201
[Asserts]
jsonpath "$.message" == "Grade created successfully"
[Captures]
grade_id: jsonpath "$.grade_id"


GET http://localhost:3000/api/grades/{{grade_id}}
HTTP 200
[Asserts]
jsonpath "$.created_at" == 2024
jsonpath "$.subject" == "Polski"
jsonpath "$.teacher_surname" == "Tomaszewska"
jsonpath "$.teacher_first_name" == "Aurelia"
jsonpath "$.student_surname" == "Borowska"
jsonpath "$.student_first_name" == "Klara"
jsonpath "$.score" == 4
jsonpath "$.id" == {{grade_id}}


PUT http://localhost:3000/api/grades/{{grade_id}}
{
  "student_id": 2,
  "teacher_id": 1,
  "score": 5,
  "created_at": 2024
}
HTTP 200


DELETE http://localhost:3000/api/grades/{{grade_id}}
HTTP 200
