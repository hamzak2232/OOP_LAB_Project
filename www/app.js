const app = {
    user: null, // Stores {id, name, role}

    init: () => {
        document.getElementById('login-form').addEventListener('submit', app.handleLogin);
        document.getElementById('logout-btn').addEventListener('click', app.handleLogout);
    },

    handleLogin: async (e) => {
        e.preventDefault();
        const id = document.getElementById('uid').value;
        const pass = document.getElementById('pass').value;

        try {
            const response = await fetch('/api/login', {
                method: 'POST',
                body: JSON.stringify({ id: id, password: pass })
            });
            const data = await response.json();

            if (data.status === 'success') {
                app.user = data;
                app.loadDashboard();
            } else {
                document.getElementById('login-msg').innerText = "Login Failed: " + (data.message || "Unknown error");
                document.getElementById('login-msg').style.color = 'red';
            }
        } catch (err) { console.error(err); }
    },

    loadDashboard: async () => {
        // UI Setup
        document.getElementById('user-name-display').innerText = app.user.name;
        document.getElementById('user-role-display').innerText = app.user.role;
        document.getElementById('login-screen').classList.remove('active');
        document.getElementById('dashboard-screen').classList.add('active');

        // Render Sidebar based on Role
        const menu = document.getElementById('sidebar-menu');
        menu.innerHTML = '<li onclick="app.showSection(\'courses\')">My Courses</li>'; // Everyone has courses view

        if (app.user.role === 'Student') {
            menu.innerHTML += '<li onclick="app.showSection(\'enroll\')">Register Course</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'attendance\')">My Attendance</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'grading\')">My Grades</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'fee\')">My Fee Challans</li>';
        } else if (app.user.role === 'Admin') {
            menu.innerHTML += '<li onclick="app.showSection(\'admin-create\')">Create Course</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'admin-add-student\')">Add Student</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'admin-add-faculty\')">Add Faculty</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'admin-fee\')">Fee Management</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'seating\')">Assign Seating</li>';
        } else if (app.user.role === 'Faculty') {
            menu.innerHTML += '<li onclick="app.showSection(\'faculty-assign\')">Add Assignment</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'attendance\')">Mark Attendance</li>';
            menu.innerHTML += '<li onclick="app.showSection(\'grading\')">Grading</li>';
        }

        // Load Course Data
        app.fetchCourses();
    },

    lastCourses: [],

    fetchCourses: async () => {
        try {
            const res = await fetch(`/api/dashboard?id=${app.user.id}`);
            const data = await res.json();
            const tbody = document.getElementById('course-table-body');
            tbody.innerHTML = '';

            if (data.courses.length === 0) tbody.innerHTML = '<tr><td colspan="10" style="text-align:center;">No courses found.</td></tr>';

            data.courses.forEach((c, index) => {
                const feeClass = c.feeStatus === 'Paid' ? 'badge-success' : 'badge-warning';
                tbody.innerHTML += `
                    <tr>
                        <td>${index + 1}</td>
                        <td>${c.code}</td>
                        <td>${c.name}</td>
                        <td>${c.creditHours || 3}</td>
                        <td>${c.major || 'N/A'}</td>
                        <td>${c.offeredTitle || c.name}</td>
                        <td>${c.classSection || ''}</td>
                        <td>${c.instructor || 'TBD'}</td>
                        <td><span class="badge ${feeClass}">${c.feeStatus || 'Pending'}</span></td>
                        <td>
                            <button class="btn-sm" onclick="app.viewDetails('${c.code}')">Details</button>
                            ${app.user.role === 'Student' ? `<button class="btn-sm" style="margin-left:5px; background:#666;" onclick="app.showSeating('${c.code}')">Seating</button>` : ''}
                            ${app.user.role === 'Admin' ? `
                                <button class="btn-sm" style="margin-left:5px; background:#007bff;" onclick="app.editCourse('${c.code}')">Edit</button>
                                <button class="btn-sm" style="margin-left:5px; background:#dc3545;" onclick="app.deleteCourse('${c.code}')">Del</button>
                            ` : ''}
                        </td>
                    </tr>
                `;
            });
            app.lastCourses = data.courses;
        } catch (e) { console.error(e); }
    },

    showSection: (section) => {
        // Hide all
        document.querySelectorAll('.section').forEach(d => d.style.display = 'none');
        document.getElementById('welcome-banner').style.display = 'none';

        // Show target
        if (section === 'courses') {
            document.getElementById('courses-section').style.display = 'block';
            app.fetchCourses(); // Refresh
        } else if (section === 'enroll') {
            document.getElementById('enroll-section').style.display = 'block';
            app.fetchAvailableCourses(); // New: Load list
        } else if (section === 'admin-create') {
            document.getElementById('admin-create-section').style.display = 'block';
        } else if (section === 'admin-add-student') {
            document.getElementById('admin-add-student-section').style.display = 'block';
        } else if (section === 'admin-add-faculty') {
            document.getElementById('admin-add-faculty-section').style.display = 'block';
        } else if (section === 'admin-fee') {
            document.getElementById('admin-fee-section').style.display = 'block';
            document.getElementById('admin-fee-controls').style.display = 'block';
            app.fetchFeeStatus(); // Load student fee data
        } else if (section === 'faculty-assign') {
            document.getElementById('faculty-assign-section').style.display = 'block';
        } else if (section === 'attendance') {
            document.getElementById('attendance-section').style.display = 'block';
            if (app.user.role === 'Faculty') {
                document.getElementById('faculty-attendance-controls').style.display = 'block';
            }
            app.fetchAttendance();
        } else if (section === 'grading') {
            document.getElementById('grading-section').style.display = 'block';
            if (app.user.role === 'Faculty') {
                document.getElementById('faculty-grading-controls').style.display = 'block';
            }
            app.fetchGrades();
        } else if (section === 'seating') {
            document.getElementById('seating-section').style.display = 'block';
            if (app.user.role === 'Admin') {
                document.getElementById('admin-seating-controls').style.display = 'block';
            }
        } else if (section === 'fee') {
            document.getElementById('fee-section').style.display = 'block';
            app.fetchStudentFees();
        }
    },

    // Attendance Handlers
    markAttendance: async () => {
        const code = document.getElementById('att-course-code').value;
        const sid = document.getElementById('att-student-id').value;
        const date = document.getElementById('att-date').value;
        const present = document.getElementById('att-status').value === 'true';
        await app.post('/api/attendance', { courseCode: code, studentId: sid, date, present });
        app.fetchAttendance();
    },

    fetchAttendance: async () => {
        try {
            const res = await fetch(`/api/attendance?studentId=${app.user.id}`);
            const data = await res.json();
            const tbody = document.getElementById('attendance-table-body');
            tbody.innerHTML = '';
            if (!data.attendance) return;
            data.attendance.forEach(a => {
                tbody.innerHTML += `<tr><td>${a.date}</td><td>${a.course}</td><td>${a.status}</td></tr>`;
            });
        } catch (e) { console.error(e); }
    },

    // Grading Handlers
    setGrade: async () => {
        const code = document.getElementById('grade-course-code').value;
        const sid = document.getElementById('grade-student-id').value;
        const mid = document.getElementById('grade-mid').value;
        const fin = document.getElementById('grade-final').value;
        const asg = document.getElementById('grade-asg').value;
        const quiz = document.getElementById('grade-quiz').value;
        await app.post('/api/grades', { courseCode: code, studentId: sid, midterm: mid, finalExam: fin, assignments: asg, quizzes: quiz });
        app.fetchGrades();
    },

    fetchGrades: async () => {
        try {
            // This is a bit complex since getGradeData returns one record
            // For demo, we just try to fetch for the current user and some courses
            const res = await fetch(`/api/grades?studentId=${app.user.id}`);
            const g = await res.json();
            const tbody = document.getElementById('grading-table-body');
            tbody.innerHTML = '';
            if (g.error) return;
            tbody.innerHTML = `
                <tr>
                    <td>${app.user.id} Record</td>
                    <td>${g.midterm}</td>
                    <td>${g.finalExam}</td>
                    <td>${g.assignments}</td>
                    <td>${g.quizzes}</td>
                    <td>${g.total}</td>
                    <td>${g.letterGrade}</td>
                    <td>${g.gpa}</td>
                </tr>
            `;
        } catch (e) { console.error(e); }
    },

    // Seating Handlers
    generateSeating: async () => {
        const code = document.getElementById('seat-course-code').value;
        const eid = document.getElementById('seat-exam-id').value;
        const date = document.getElementById('seat-date').value;
        const room = document.getElementById('seat-room').value;
        await app.post('/api/seating-plan', { courseCode: code, examId: eid, date, room });
        alert('Seating Plan Generated!');
    },

    showSeating: async (code) => {
        try {
            const res = await fetch(`/api/seating-plan?code=${code}`);
            const data = await res.json();
            if (data.error) { alert('No seating plan found for this course.'); return; }

            app.showSection('seating');
            const card = document.getElementById('seating-display-card');
            card.style.display = 'block';
            document.getElementById('seat-info').innerText = `${data.examId} | Room: ${data.room} | Date: ${data.date}`;

            const tbody = document.getElementById('seating-table-body');
            tbody.innerHTML = '';
            data.seats.forEach(s => {
                tbody.innerHTML += `<tr><td>${s.studentId}</td><td>Seat ${s.seat}</td></tr>`;
            });
        } catch (e) { console.error(e); }
    },

    // Fee Handlers
    generateChallan: async () => {
        const sid = document.getElementById('fee-student-id').value;
        const sem = document.getElementById('fee-semester').value;
        const tui = document.getElementById('fee-tuition').value;
        const lab = document.getElementById('fee-lab').value;
        const lib = document.getElementById('fee-library').value;
        const due = document.getElementById('fee-due').value;
        await app.post('/api/fee-challan', { studentId: sid, semester: sem, tuition: tui, lab, library: lib, dueDate: due });
        app.fetchFeeStatus();
    },

    fetchStudentFees: async () => {
        try {
            const res = await fetch(`/api/fee-challan?studentId=${app.user.id}`);
            const data = await res.json();
            const tbody = document.getElementById('fee-table-body');
            tbody.innerHTML = '';
            if (!data.challans) return;
            data.challans.forEach(f => {
                tbody.innerHTML += `<tr><td>${f.id}</td><td>${f.semester}</td><td>${f.total}</td><td>${f.due}</td><td>${f.status}</td></tr>`;
            });
        } catch (e) { console.error(e); }
    },

    // New: Fetch and Render All Courses for Registration
    fetchAvailableCourses: async () => {
        try {
            const res = await fetch('/api/courses');
            const data = await res.json();
            const tbody = document.getElementById('available-course-table-body');
            tbody.innerHTML = '';

            if (data.courses.length === 0) tbody.innerHTML = '<tr><td colspan="8" style="text-align:center;">No courses available.</td></tr>';

            data.courses.forEach((c, index) => {
                tbody.innerHTML += `
                    <tr>
                        <td>${index + 1}</td>
                        <td>${c.code}</td>
                        <td>${c.name}</td>
                        <td>${c.creditHours || 3}</td>
                        <td>${c.major || 'N/A'}</td>
                        <td>${c.classSection || ''}</td>
                        <td>${c.instructor || 'TBD'}</td>
                        <td>
                            <button class="btn-sm" onclick="app.viewDetails('${c.code}')">View</button>
                            <button class="btn-primary" style="margin-left:5px;" onclick="app.enrollByCode('${c.code}')">Enroll</button>
                        </td>
                    </tr>
                `;
            });
        } catch (e) { console.error(e); }
    },

    // Details Modal
    viewDetails: async (code) => {
        try {
            const res = await fetch(`/api/course/details?code=${code}`);
            const data = await res.json();

            document.getElementById('modal-title').innerText = `${data.code} - ${data.name}`;
            document.getElementById('modal-instructor').innerText = data.instructor;

            const list = document.getElementById('modal-assignments-list');
            list.innerHTML = '';
            if (data.assignments.length === 0) list.innerHTML = '<li>No Assignments</li>';

            data.assignments.forEach(a => {
                list.innerHTML += `<li><span>${a.title}</span> <span>${a.marks} Marks (Due: ${a.deadline})</span></li>`;
            });

            document.getElementById('details-modal').classList.add('active');
        } catch (e) { console.error(e); }
    },

    closeModal: () => {
        document.getElementById('details-modal').classList.remove('active');
    },

    // ACTIONS
    createCourse: async () => {
        const code = document.getElementById('new-course-code').value;
        const name = document.getElementById('new-course-name').value;
        const offeredTitle = document.getElementById('new-course-offered-title').value || name;
        const credits = document.getElementById('new-course-credits').value || '3';
        const major = document.getElementById('new-course-major').value || 'N/A';
        const section = document.getElementById('new-course-section').value;
        const semester = document.getElementById('new-course-semester').value;
        const teacherId = document.getElementById('new-course-teacher').value;

        await app.post('/api/course', {
            code,
            name,
            offeredTitle,
            creditHours: credits.toString(),
            major,
            classSection: section,
            semester,
            teacherId
        });
    },

    enroll: async () => {
        const code = document.getElementById('enroll-code').value;
        await app.post('/api/enroll', { studentId: app.user.id, courseCode: code });
    },

    enrollByCode: async (code) => {
        await app.post('/api/enroll', { studentId: app.user.id, courseCode: code });
    },

    addAssignment: async () => {
        const code = document.getElementById('asg-course-code').value;
        const title = document.getElementById('asg-title').value;
        const marks = document.getElementById('asg-marks').value;
        const date = document.getElementById('asg-date').value;
        await app.post('/api/assignment', { courseCode: code, title, marks, date });
    },

    addStudent: async () => {
        const data = {
            id: document.getElementById('student-id').value,
            name: document.getElementById('student-name').value,
            password: document.getElementById('student-password').value,
            enrollmentNo: document.getElementById('student-enrollment').value,
            registrationNo: document.getElementById('student-registration').value,
            fatherName: document.getElementById('student-father').value,
            program: document.getElementById('student-program').value,
            intakeSemester: document.getElementById('student-intake').value,
            maxSemester: document.getElementById('student-maxsem').value,
            degreeDuration: document.getElementById('student-duration').value,
            mobileNo: document.getElementById('student-mobile').value,
            phoneNo: document.getElementById('student-phone').value,
            personalEmail: document.getElementById('student-personal-email').value,
            universityEmail: document.getElementById('student-uni-email').value,
            department: document.getElementById('student-department').value,
            currentAddress: document.getElementById('student-current-address').value,
            permanentAddress: document.getElementById('student-permanent-address').value
        };

        await app.post('/api/admin/add-student', data);
    },

    downloadUserReport: async () => {
        try {
            const res = await fetch('/api/admin/report');
            const text = await res.text();

            const blob = new Blob([text], { type: 'text/plain' });
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = 'organized_user_report.txt';
            document.body.appendChild(a);
            a.click();
            window.URL.revokeObjectURL(url);
            document.body.removeChild(a);
        } catch (e) { alert('Failed to download report'); }
    },

    addFaculty: async () => {
        const data = {
            id: document.getElementById('faculty-id').value,
            name: document.getElementById('faculty-name').value,
            password: document.getElementById('faculty-password').value,
            employeeId: document.getElementById('faculty-emp-id').value,
            department: document.getElementById('faculty-department').value,
            designation: document.getElementById('faculty-designation').value,
            officeHours: document.getElementById('faculty-office-hours').value,
            teachableCourses: document.getElementById('faculty-teachable').value,
            mobileNo: document.getElementById('faculty-mobile').value,
            phoneNo: document.getElementById('faculty-phone').value,
            personalEmail: document.getElementById('faculty-personal-email').value,
            universityEmail: document.getElementById('faculty-uni-email').value,
            currentAddress: document.getElementById('faculty-current-address').value,
            permanentAddress: document.getElementById('faculty-permanent-address').value
        };

        await app.post('/api/admin/add-faculty', data);
    },

    post: async (url, data) => {
        try {
            const res = await fetch(url, {
                method: 'POST', body: JSON.stringify(data)
            });
            const json = await res.json();
            if (json.status === 'success') {
                alert('Action Successful!');
                if (app.user.role !== 'Admin') app.fetchCourses(); // Refresh list usually
                // Refresh list if we are in enroll view
                if (app.user.role === 'Student' && document.getElementById('enroll-section').style.display === 'block') {
                    app.fetchAvailableCourses();
                }
            } else {
                alert('Error: ' + (json.message || 'Failed'));
            }
        } catch (e) { alert('Network Error'); }
    },

    handleLogout: () => {
        location.reload();
    },

    fetchFeeStatus: async () => {
        try {
            const res = await fetch('/api/admin/fee-status');
            const data = await res.json();
            const tbody = document.getElementById('fee-table-body');
            tbody.innerHTML = '';

            if (data.students.length === 0) {
                tbody.innerHTML = '<tr><td colspan="7" style="text-align:center;">No students found.</td></tr>';
                return;
            }

            data.students.forEach((s, index) => {
                const feeClass = s.feeStatus === 'Paid' ? 'badge-success' : 'badge-warning';
                tbody.innerHTML += `
                    <tr>
                        <td>${index + 1}</td>
                        <td>${s.id}</td>
                        <td>${s.name}</td>
                        <td>${s.program || 'N/A'}</td>
                        <td>${s.intakeSemester || 'N/A'}</td>
                        <td><span class="badge ${feeClass}">${s.feeStatus || 'Pending'}</span></td>
                        <td><button class="btn-sm" onclick="app.toggleFeeStatus('${s.id}')">Toggle Status</button></td>
                    </tr>
                `;
            });
        } catch (e) { console.error(e); }
    },

    toggleFeeStatus: async (studentId) => {
        await app.post('/api/admin/toggle-fee', { studentId });
        app.fetchFeeStatus(); // Refresh
    },

    // COURSE MANAGEMENT Actions
    deleteCourse: async (code) => {
        if (!confirm(`Are you sure you want to delete course ${code}?`)) return;
        await app.post('/api/course/delete', { code });
        app.fetchCourses();
    },

    editCourse: (code) => {
        const c = app.lastCourses.find(x => x.code === code);
        if (!c) return;

        document.getElementById('edit-course-code-hidden').value = c.code;
        document.getElementById('edit-course-name').value = c.name;
        document.getElementById('edit-course-offered-title').value = c.offeredTitle || c.name;
        document.getElementById('edit-course-credits').value = c.creditHours || 3;
        document.getElementById('edit-course-major').value = c.major || 'N/A';
        document.getElementById('edit-course-section').value = c.classSection || '';
        document.getElementById('edit-course-semester').value = c.semester || '';
        document.getElementById('edit-course-teacher').value = c.instructorId || '';

        document.getElementById('edit-course-modal').classList.add('active');
    },

    closeEditModal: () => {
        document.getElementById('edit-course-modal').classList.remove('active');
    },

    submitCourseUpdate: async () => {
        const data = {
            code: document.getElementById('edit-course-code-hidden').value,
            name: document.getElementById('edit-course-name').value,
            offeredTitle: document.getElementById('edit-course-offered-title').value,
            creditHours: document.getElementById('edit-course-credits').value,
            major: document.getElementById('edit-course-major').value,
            classSection: document.getElementById('edit-course-section').value,
            semester: document.getElementById('edit-course-semester').value,
            teacherId: document.getElementById('edit-course-teacher').value
        };

        const res = await fetch('/api/course/update', {
            method: 'POST', body: JSON.stringify(data)
        });
        const json = await res.json();
        if (json.status === 'success') {
            alert('Course Updated!');
            app.closeEditModal();
            app.fetchCourses();
        } else {
            alert('Error: ' + json.message);
        }
    },
};

document.addEventListener('DOMContentLoaded', app.init);

