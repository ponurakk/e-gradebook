GET http://localhost:3000/api/teachers
HTTP 200
[Asserts]
jsonpath "$[0].id" == 1
jsonpath "$[0].first_name" == "Aurelia"
jsonpath "$[0].surname" == "Tomaszewska"
jsonpath "$[0].subject" == "Polski"
jsonpath "$[0].teacher_of" == 1


POST http://localhost:3000/api/teachers
{
  "first_name": "Maja",
  "surname": "Nowak",
  "subject": "Matematyka",
  "teacher_of": 3
}
HTTP 201
[Asserts]
jsonpath "$.message" == "Teacher created successfully"
[Captures]
teacher_id: jsonpath "$.teacher_id"


GET http://localhost:3000/api/teachers/{{teacher_id}}
HTTP 200
[Asserts]
jsonpath "$.id" == {{teacher_id}}
jsonpath "$.first_name" == "Maja"
jsonpath "$.surname" == "Nowak"
jsonpath "$.subject" == "Matematyka"
jsonpath "$.teacher_of" == 3


PUT http://localhost:3000/api/teachers/{{teacher_id}}
{
  "first_name": "Maja",
  "surname": "Nowak",
  "subject": "Polski",
  "teacher_of": 3
}
HTTP 200


DELETE http://localhost:3000/api/teachers/{{teacher_id}}
HTTP 200
