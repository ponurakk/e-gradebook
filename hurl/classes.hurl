GET http://localhost:3000/api/classes
HTTP 200
[Asserts]
jsonpath "$[0].name" == "1ct"
jsonpath "$[0].id" == 1


POST http://localhost:3000/api/classes
{
  "name": "3ct"
}
HTTP 201
[Asserts]
jsonpath "$.message" == "Class created successfully"
[Captures]
class_id: jsonpath "$.class_id"


GET http://localhost:3000/api/classes/{{class_id}}
HTTP 200
[Asserts]
jsonpath "$.id" == {{class_id}}
jsonpath "$.name" == "3ct"


PUT http://localhost:3000/api/classes/{{class_id}}
{
  "name": "new_name"
}
HTTP 200


DELETE http://localhost:3000/api/classes/{{class_id}}
HTTP 200
