SELECT name,mark FROM Students INNER JOIN Enrolments 
ON Students.student_id = Enrolments.student_id 
WHERE subject_code = 'COMP SCI 2000' AND mark < 50;