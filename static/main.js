const { dialog, div, h1, h2, button, form, input, p, label, a, section, span, select, option } = van.tags;

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

// Utility function for DELETE requests
async function deleteAPI(url, id) {
  const response = await fetch(`${url}/${id}`, {
    method: 'DELETE',
  });
  fetchStudents();
  fetchGrades();
  fetchClasses();
  fetchTeachers();
  return response.json();
}

// Utility function for PUT requests
async function putAPI(url, data) {
  const response = await fetch(url, {
    method: 'PUT',
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


const AddStudentForm = () => {
  const student_name = van.state("");
  const student_surname = van.state("");
  const student_class = van.state("");
  return div(
    { class: "mb-4 inline-block" },
    input({
      type: "text",
      name: "name",
      placeholder: "Student First-name",
      class: "border p-2 mb-2 w-[300px] block",
      value: student_name,
      oninput: e => student_name.val = e.target.value
    }),
    input({
      type: "text",
      name: "name",
      placeholder: "Student Surname",
      class: "border p-2 mb-2 w-[300px] block",
      value: student_surname,
      oninput: e => student_surname.val = e.target.value
    }),
    van.derive(() => select({
      class: "border p-2 mb-2 w-[300px] block",
      value: student_class,
      oninput: e => student_class.val = e.target.value
    }, [option({ value: "0", selected: true, disabled: true }, "Select class"), ...classesState.val.map(v => option({ value: v.id }, v.name))])),
    button({
      type: "submit",
      class: "bg-green-500 text-white px-4 py-2",
      onclick: async () => {
        await postAPI(STUDENTS_URL, {
          first_name: student_name.val,
          surname: student_surname.val,
          class: student_class.val,
        });
        await fetchStudents();
      }
    }, "Add Student")
  );
}

const AddClassForm = () => {
  const class_name = van.state("");
  return div(
    { class: "mb-4 inline-block" },
    input({
      type: "text",
      name: "name",
      placeholder: "Class",
      class: "border p-2 mb-2 w-[300px] block",
      value: class_name,
      oninput: e => class_name.val = e.target.value
    }),
    button({
      type: "submit",
      class: "bg-green-500 text-white px-4 py-2",
      onclick: async () => {
        await postAPI(CLASSES_URL, {
          name: class_name.val
        });
        await fetchClasses();
      }
    }, "Add Class")
  );
}

const AddGradeForm = () => {
  const student_id = van.state("");
  const teacher_id = van.state("");
  const grade = van.state(1);
  return div(
    { class: "mb-4" },
    van.derive(() => select({
      class: "border p-2 mb-2 w-[300px] block",
      value: student_id,
      oninput: e => student_id.val = e.target.value
    }, [option({ value: "0", selected: true, disabled: true }, "Select Student"), ...studentsState.val.map(v => option({ value: v.id }, `${v.first_name} ${v.surname} ${v.class_name}`))])),
    van.derive(() => select({
      class: "border p-2 mb-2 w-[300px] block",
      value: teacher_id,
      oninput: e => teacher_id.val = e.target.value
    }, [option({ value: "0", selected: true, disabled: true }, "Select Teacher"), ...teachersState.val.map(v => option({ value: v.id }, `${v.first_name} ${v.surname} ${v.teacher_of}`))])),
    input({
      type: "number",
      min: 1,
      max: 6,
      name: "name",
      placeholder: "Grade/Score",
      class: "border p-2 mb-2 w-[300px] block",
      value: grade,
      oninput: e => grade.val = e.target.value
    }),
    button({
      type: "submit",
      class: "bg-green-500 text-white px-4 py-2",
      onclick: async () => {
        await postAPI(GRADES_URL, {
          student_id: parseInt(student_id.val),
          teacher_id: parseInt(teacher_id.val),
          score: parseInt(grade.val),
          created_at: "2024",
        });
        await fetchGrades();
      }
    }, "Add Grade")
  );
}

const AddTeacherForm = () => {
  const teacher_name = van.state("");
  const teacher_surname = van.state("");
  const teacher_subject = van.state("");
  const teacher_class = van.state("");
  return div(
    { class: "mb-4" },
    input({
      type: "text",
      name: "name",
      placeholder: "Teacher First-name",
      class: "border p-2 mb-2 w-[400px] block",
      value: teacher_name,
      oninput: e => teacher_name.val = e.target.value
    }),
    input({
      type: "text",
      name: "name",
      placeholder: "Teacher Surname",
      class: "border p-2 mb-2 w-[400px] block",
      value: teacher_surname,
      oninput: e => teacher_surname.val = e.target.value
    }),
    input({
      type: "text",
      name: "name",
      placeholder: "Subject Specialization",
      class: "border p-2 mb-2 w-[400px] block",
      value: teacher_subject,
      oninput: e => teacher_subject.val = e.target.value
    }),
    van.derive(() => select({
      class: "border p-2 mb-2 w-[400px] block",
      value: teacher_class,
      oninput: e => teacher_class.val = e.target.value
    }, [option({ value: "0", selected: true, disabled: true }, "Select class"), ...classesState.val.map(v => option({ value: v.id }, v.name))])),
    button({
      type: "submit",
      class: "bg-green-500 text-white px-4 py-2",
      onclick: async () => {
        await postAPI(TEACHERS_URL, {
          first_name: teacher_name.val,
          surname: teacher_surname.val,
          subject: teacher_subject.val,
          teacher_of: teacher_class.val,
        });
        await fetchTeachers();
      }
    }, "Add Teacher")
  );
}

const StudentDialog = ({ id, student_name, student_surname, student_class }) => {
  return dialog(
    { id: `modal-${id}`, class: "p-4 bg-white rounded shadow-md w-1/3 z-20 fixed hidden" },
    input({
      type: "text",
      value: student_name.val,
      class: "border p-2 mb-2 w-full",
      oninput: e => student_name.val = e.target.value
    }),
    input({
      type: "text",
      value: student_surname.val,
      class: "border p-2 mb-2 w-full",
      oninput: e => student_surname.val = e.target.value
    }),
    van.derive(() => select({
      class: "border p-2 mb-2 w-full",
      value: student_class.val,
      oninput: e => student_class.val = e.target.value
    }, [option({ value: "0", selected: true, disabled: true }, "Select class"), ...classesState.val.map(v => option({ value: v.id }, v.name))])),
    button({
      type: "button",
      class: "bg-yellow-500 text-white px-4 py-2 ml-2",
      onclick: async () => {
        await putAPI(`${STUDENTS_URL}/${id}`, {
          first_name: student_name.val,
          surname: student_surname.val,
          class: student_class.val,
        });
        await fetchStudents();
        closeModal(`modal-${id}`);
      }
    }, "Save"),
    button({
      class: "bg-black text-white px-4 py-2 ml-2",
      onclick: (e) => { closeModal(e.target.parentNode.id); }
    }, "Close")
  );
}

const Student = ({ id, name }) => {
  const handleDelete = async () => {
    await deleteAPI(STUDENTS_URL, id);
    await fetchStudents();
  };
  const student_name = van.state("");
  const student_surname = van.state("");
  const student_class = van.state("");
  return div(
    { class: "flex justify-between items-center bg-white p-4 shadow w-max" },
    span({ class: "mr-10" }, name),
    div(
      div(
        { class: "inline-block" },
        button({
          type: "button",
          class: "bg-yellow-500 text-white px-4 py-2",
          onclick: () => openModal(`modal-${id}`)
        }, "Edit")
      ),
      div(
        { class: "inline-block" },
        button({ type: "button", class: "bg-red-500 text-white px-4 py-2", onclick: handleDelete }, "Delete")
      ),
      van.derive(() => StudentDialog({ id, student_name, student_surname, student_class }))
    )
  );
};

const Class = ({ id, name }) => {
  const handleDelete = async () => {
    await deleteAPI(CLASSES_URL, id);
    await fetchClasses(); // Update classes after deletion
  };

  return div(
    { class: "flex justify-between items-center bg-white p-4 shadow w-max" },
    span({ class: "mr-10" }, name),
    div(
      div(
        { class: "inline-block" },
        button({
          type: "button",
          class: "bg-yellow-500 text-white px-4 py-2",
          onclick: () => openModal(`modal-${id}`)
        }, "Edit")
      ),
      div(
        { class: "inline-block" },
        button({ type: "button", class: "bg-red-500 text-white px-4 py-2", onclick: handleDelete }, "Delete")
      ),
      dialog(
        { id: `modal-${id}`, class: "p-4 bg-white rounded shadow-md w-1/3" },
        input({ type: "text", value: name, class: "border p-2" }),
        button({ type: "button", class: "bg-yellow-500 text-white px-4 py-2 ml-2" }, "Save"),
        button({
          class: "bg-black text-white px-4 py-2 ml-2",
          onclick: (e) => { closeModal(e.target.parentNode.id); }
        }, "Close")
      ),
    )
  );
};

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
  studentsState.val.map(({ id, first_name, surname, class_name }) => Student({
    id,
    name: `${first_name} ${surname} ${class_name}`,
  }))
));

const ClassesList = van.derive(() => div(
  classesState.val.map(({ id, name }) => Class({
    id,
    name,
  }))
));

const Grade = ({ id, subject, score }) => {
  const handleDelete = async () => {
    await deleteAPI(GRADES_URL, id);
    await fetchGrades(); // Update grades after deletion
  };

  return div(
    { class: "flex justify-between items-center bg-white p-4 shadow w-max" },
    span({ class: "mr-10" }, `${subject} ${score}`),
    div(
      div(
        { class: "inline-block" },
        button({
          type: "button",
          class: "bg-yellow-500 text-white px-4 py-2",
          onclick: () => openModal(`modal-${id}`)
        }, "Edit")
      ),
      div(
        { class: "inline-block" },
        button({ type: "button", class: "bg-red-500 text-white px-4 py-2", onclick: handleDelete }, "Delete")
      ),
      dialog(
        { id: `modal-${id}`, class: "p-4 bg-white rounded shadow-md w-1/3" },
        input({ type: "text", value: `${subject} ${score}`, class: "border p-2" }),
        button({ type: "button", class: "bg-yellow-500 text-white px-4 py-2 ml-2" }, "Save"),
        button({
          class: "bg-black text-white px-4 py-2 ml-2",
          onclick: (e) => { closeModal(e.target.parentNode.id); }
        }, "Close")
      ),
    )
  );
};

const GradesList = van.derive(() => div(
  gradesState.val.map(({ id, subject, score }) => Grade({
    id,
    subject,
    score,
  }))
));

const Teacher = ({ id, first_name, surname, teacher_of }) => {
  const handleDelete = async () => {
    await deleteAPI(TEACHERS_URL, id);
    await fetchTeachers();
  };

  return div(
    { class: "flex justify-between items-center bg-white p-4 shadow w-max" },
    span({ class: "mr-10" }, `${first_name} ${surname} ${teacher_of}`),
    div(
      div(
        { class: "inline-block" },
        button({
          type: "button",
          class: "bg-yellow-500 text-white px-4 py-2",
          onclick: () => openModal(`modal-${id}`)
        }, "Edit")
      ),
      div(
        { class: "inline-block" },
        button({ type: "button", class: "bg-red-500 text-white px-4 py-2", onclick: handleDelete }, "Delete")
      ),
      dialog(
        { id: `modal-${id}`, class: "p-4 bg-white rounded shadow-md w-1/3" },
        input({ type: "text", value: `${first_name} ${surname} ${teacher_of}`, class: "border p-2" }),
        button({ type: "button", class: "bg-yellow-500 text-white px-4 py-2 ml-2" }, "Save"),
        button({
          class: "bg-black text-white px-4 py-2 ml-2",
          onclick: (e) => { closeModal(e.target.parentNode.id); }
        }, "Close")
      ),
    )
  );
};

const TeachersList = van.derive(() => div(
  teachersState.val.map(({ id, first_name, surname, teacher_of }) => Teacher({
    id,
    first_name,
    surname,
    teacher_of,
  }))
));

const App = () =>
  div(
    div({ id: "modal-bg", class: "hidden fixed top-0 left-0 w-full h-full backdrop-blur-sm" }),
    div(
      { class: "flex flex-wrap gap-10 justify-between" },
      Section({ name: "Students", form: AddStudentForm(), items: StudentsList }),
      Section({ name: "Classes", form: AddClassForm(), items: ClassesList }),
      Section({ name: "Grades", form: AddGradeForm(), items: GradesList }),
      Section({ name: "Teachers", form: AddTeacherForm(), items: TeachersList }),
    )
  );

van.add(document.getElementById("app"), App());
fetchStudents();
fetchGrades();
fetchClasses();
fetchTeachers();
