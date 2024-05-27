const { dialog, div, h1, h2, button, form, input, p, label, nav, a, section, span } = van.tags;

const API_BASE_URL = '/api';
const STUDENTS_URL = `${API_BASE_URL}/students`;
const GRADES_URL = `${API_BASE_URL}/grades`;
const CLASSES_URL = `${API_BASE_URL}/classes`;
const TEACHERS_URL = `${API_BASE_URL}/teachers`;

// Utility function for GET requests
async function fetchAPI(url) {
  const response = await fetch(url);
  return response.json();
}

// Utility function for POST requests
async function postAPI(url, data) {
  const response = await fetch(url, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(data),
  });
  return response.json();
}

function openModal(id) {
  document.getElementById("modal-bg").style.display = "block";
  document.getElementById(id).showModal();
}

function closeModal(id) {
  document.getElementById("modal-bg").style.display = "none";
  document.getElementById(id).close();
}

const studentsState = van.state([]);
const gradesState = van.state([]);
const classesState = van.state([]);
const teachersState = van.state([]);

async function fetchStudents() {
  const students = await fetchAPI(STUDENTS_URL);
  studentsState.val = students;
}

async function fetchGrades() {
  const grades = await fetchAPI(GRADES_URL);
  gradesState.val = grades;
}

async function fetchClasses() {
  const classes = await fetchAPI(CLASSES_URL);
  classesState.val = classes;
}

async function fetchTeachers() {
  const teachers = await fetchAPI(TEACHERS_URL);
  teachersState.val = teachers;
}


const Navbar = () => {
  let items = ["Students", "Classes", "Grades", "Teachers"];
  return nav(
    { class: "flex gap-3" },
    items.map(v => a({ href: `#${v.toLowerCase()}`, class: "bg-blue-500 text-white px-4 py-2" }, v))
  );
};

const AddStudentForm = () => {
  return div(
    { class: "mb-4 inline-block" },
    input({ type: "text", name: "name", placeholder: "Student First-name", class: "border p-2 mb-2 w-[300px] block" }),
    input({ type: "text", name: "name", placeholder: "Student Surname", class: "border p-2 mb-2 w-[300px] block" }),
    input({ type: "text", name: "name", placeholder: "Student Class", class: "border p-2 mb-2 w-[300px] block" }),
    button({ type: "submit", class: "bg-green-500 text-white px-4 py-2" }, "Add Student")
  );
}

const AddClassForm = () => {
  return div(
    { class: "mb-4 inline-block" },
    input({ type: "text", name: "name", placeholder: "Class", class: "border p-2 mb-2 w-[300px] block" }),
    button({ type: "submit", class: "bg-green-500 text-white px-4 py-2" }, "Add Class")
  );
}

const AddGradeForm = () => {
  return div(
    { class: "mb-4" },
    input({ type: "text", name: "name", placeholder: "Student Id", class: "border p-2 mb-2 w-[300px] block" }),
    input({ type: "text", name: "name", placeholder: "Teacher Id", class: "border p-2 mb-2 w-[300px] block" }),
    input({ type: "text", name: "name", placeholder: "Grade/Score", class: "border p-2 mb-2 w-[300px] block" }),
    input({ type: "text", name: "name", placeholder: "Year", class: "border p-2 mb-2 w-[300px] block" }),
    button({ type: "submit", class: "bg-green-500 text-white px-4 py-2" }, "Add Grade")
  );
}

const AddTeacherForm = () => {
  return div(
    { class: "mb-4" },
    input({ type: "text", name: "name", placeholder: "Teacher First-name", class: "border p-2 mb-2 w-[400px] block" }),
    input({ type: "text", name: "name", placeholder: "Teacher Surname", class: "border p-2 mb-2 w-[400px] block" }),
    input({ type: "text", name: "name", placeholder: "Subject Specialization", class: "border p-2 mb-2 w-[400px] block" }),
    input({ type: "text", name: "name", placeholder: "Teacher Of Class", class: "border p-2 mb-2 w-[400px] block" }),
    button({ type: "submit", class: "bg-green-500 text-white px-4 py-2" }, "Add Teacher")
  );
}

const Student = ({ name }) => {
  return div(
    { class: "flex justify-between items-center bg-white p-4 shadow w-max" },
    span({ class: "mr-10" }, name),
    div(
      div(
        { class: "inline-block" },
        button({
          type: "button",
          class: "bg-yellow-500 text-white px-4 py-2",
          onclick: () => openModal(`modal-${name}`)
        }, "Edit")
      ),
      div(
        { class: "inline-block" },
        button({ type: "button", class: "bg-red-500 text-white px-4 py-2" }, "Delete")
      ),
      dialog(
        { id: `modal-${name}`, class: "p-4 bg-white rounded shadow-md w-1/3" },
        input({ type: "text", value: name, class: "border p-2" }),
        button({
          class: "bg-black text-white px-4 py-2",
          onclick: (e) => { closeModal(e.target.parentNode.id); }
        }, "Close")
      ),
    )
  );
}

const Section = ({ name, form, items }) => {
  return section(
    { id: name.toLowerCase(), class: "mt-6 mb-8 w-max align-top" },
    h2({ class: "text-xl font-bold mb-4" }, name),
    div(
      { class: "flex gap-5" },
      form,
      div(
        { class: "space-y-4" },
        items,
      ),
    )
  );
};

const StudentsList = van.derive(() => div(
  studentsState.val.map(v => Student({
    name: `${v.first_name} ${v.surname} ${v.class_name}`,
  }))
));

const ClassesList = van.derive(() => div(
  classesState.val.map(v => Student({
    name: `${v.name}`,
  }))
));

const GradesList = van.derive(() => div(
  gradesState.val.map(v => Student({
    name: `${v.subject} ${v.score}`,
  }))
));

const TeachersList = van.derive(() => div(
  teachersState.val.map(v => Student({
    name: `${v.first_name} ${v.surname} ${v.teacher_of}`,
  }))
));

const App = () =>
  div(
    div({ id: "modal-bg", class: "hidden fixed top-0 left-0 w-full h-full backdrop-blur-sm" }),
    Navbar(),
    div(
      { class: "flex flex-wrap gap-10 justify-between" },
      Section({ name: "Students", form: AddStudentForm(), items: StudentsList }),
      Section({ name: "Classes", form: AddClassForm(), items: ClassesList }),
      Section({ name: "Grades", form: AddGradeForm(), items: GradesList }),
      Section({ name: "Teachers", form: AddTeacherForm(), items: TeachersList }),
    )
  );

van.add(document.getElementById("app"), App());
// openModal("modal");
fetchStudents();
fetchGrades();
fetchClasses();
fetchTeachers();
