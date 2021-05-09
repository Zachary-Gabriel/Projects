SELECT name FROM Students INNER JOIN Enrolments 
ON Students.student_id = Enrolments.student_id 
WHERE subject_code LIKE 'COMP SCI 2207';