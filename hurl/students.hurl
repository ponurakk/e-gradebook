GET http://localhost:3000/api/students
HTTP 200
[Asserts]
jsonpath "$[0].id" == 2
jsonpath "$[0].surname" == "Borowska"
jsonpath "$[0].class" == 1
jsonpath "$[0].first_name" == "Klara"
jsonpath "$[0].class_name" == "1ct"


POST http://localhost:3000/api/students
{
  "first_name": "Gustaw",
  "surname": "Mróz",
  "class": 1
}
HTTP 201
[Asserts]
jsonpath "$.message" == "Student created successfully"
[Captures]
student_id: jsonpath "$.student_id"


GET http://localhost:3000/api/students/{{student_id}}
HTTP 200
[Asserts]
jsonpath "$.id" == {{student_id}}
jsonpath "$.first_name" == "Gustaw"
jsonpath "$.surname" == "Mróz"
jsonpath "$.class" == 1
jsonpath "$.class_name" == "1ct"


PUT http://localhost:3000/api/students/{{student_id}}
{
  "first_name": "Gustaw",
  "surname": "Mróz",
  "class": 1
}
HTTP 200


DELETE http://localhost:3000/api/students/{{student_id}}
HTTP 200
