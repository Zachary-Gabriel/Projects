// referencing the helper files
#include "EvalUCS.h"
#include "Solution.h"

#include <iostream> // for output
#include <fstream> // for output
#include <vector> // for vectors and intmatrix
#include <string> // for string commands
#include <cmath> // for floor fnctn

// to simplify the code
using namespace std;

// To filter out the slot between 12 and 1pm
void filter_lunch ()
{
	for (int i = 0; i < ProblemUCS::nL; ++i)
	{
		for (int j = 0; j < 40; ++j)
		{
			if (j % 8 == 3)
			{
				ProblemUCS::LP[i][j] = 0;
			}
		}
	}
}

// printing the resulting output to a file
void print_IntMatrix (IntMatrix mat)
{
	ofstream myfile;
	myfile.open ("output.seh");
	for (int i = 0; i < mat.size (); ++i)
	{
		for (int j = 0; j < mat.at (i).size ()-1; ++j)
		{
			myfile << mat[i][j] << ',';
		}

		myfile << mat[i][mat.at (i).size ()-1] << endl;
	}
	myfile.close ();

	return ;
}

// creating the result IntMatrix that has a blank timetable (full on -1's)
IntMatrix create_IntMatrix (int length)
{
	IntMatrix result;
	for (int i = 0; i < length; ++i)
	{
		vector<int> temp;
		for (int j = 0; j < 40; ++j)
		{
			temp.push_back (-1);
		}
		result.push_back(temp) ;
	}
	return result;
}

// making the one lecturer teach the course if only one lecturer can teach it
void parse_one (int lecturer, int course, IntMatrix& tt, IntMatrix& teachers, vector<int>& course_availabilities)
{
	// initialising variables
	vector<int> tt_line = tt.at (course);
	vector<int> course_times;
	int current_hours = 0;
	int standards = 1;
	int stop_hang = 0;
	// looping through until I've got enough hours
	while (current_hours < ProblemUCS::cHours.at (course) && stop_hang < 1000)
	{
		// each day of the week
		for (int i = 0; i < 5; ++i)
		{
			// each block of the day
			for (int j = 0; j < 8; ++j)
			{
				// checking to see if there already exists a course block on that day
				bool dont_bother = false;
				for (int k = 0; k < course_times.size (); ++k)
				{
					int day = floor (course_times.at(k) / 8 ) ;
					// if wrong day, keep going through loop
					if (day != 5*i)
					{
						continue;
					}
					
					// if right day
					// if its the class before or after
					if ( abs (abs (course_times.at(k) - 8*day) - j)  == 1)
					{
						dont_bother = false;
					}
					// if theres a distance of one between the classes, then it can't be added
					else
					{
						dont_bother = true;
						break;
					}
				}

				// checking if the teacher has already taught two courses in a row
				if (!dont_bother)
				{
					int moment = j-1;
					if (moment > 0)
					{
						// previous block
						if (teachers.at (lecturer).at (8*i + moment) == 1)
						{
							-- moment ;
							if (moment >= 0)
							{
								// 2blocks ago (i.e. needs a break)
								if (teachers.at (lecturer).at (8*i + moment) == 1)
								{
									dont_bother = true;
								}
							}
						}
					}
				}

				// if I want / if I can to take that block 
				if ( !dont_bother && (ProblemUCS::LP.at (lecturer).at (8*i+j) == standards) && (teachers.at (lecturer).at (8*i+j) == -1) && (course_availabilities.at(8*i+j) < ProblemUCS::rooms ))
				{
					// setting the timetable to the lecturer, and incrementing all my constraint variables
					tt_line.at (8*i+j) = lecturer;
					teachers.at (lecturer).at (8*i+j) = 1;
					++ course_availabilities.at(8*i+j) ;
					course_times.push_back (8*i+j);

					// get out of the loops if the timetable shouldn't be modified anymore
					++ current_hours ;
					if (current_hours == ProblemUCS::cHours.at (course))
					{
						break;
					}
				}
			}

			// get out of the loops if we shouldn't modify the timetable anymore
			if (current_hours == ProblemUCS::cHours.at (course))
			{
				break;
			}
		}
		// decreasing the lecturer's standards or increasing the variable, if it can't all be done using 1's
		++ standards ;
		++ stop_hang ;
	}
	
	// finishing up the timetable modifications
	tt.at (course) = tt_line;
	return ;
}

void parse_multiple (vector<int> lects, int course, IntMatrix& tt, IntMatrix& teachers, vector<int>& course_availabilities)
{
	
	// initialising variables
	vector<int> tt_line = tt.at (course);
	vector<int> course_times;
	int current_hours = 0;
	int standards = 1;
	int stop_hang = 0;
	// looping through until I've got enough hours
	while (current_hours < ProblemUCS::cHours.at (course) && stop_hang < 1000)
	{
		// each day of the week
		for (int i = 0; i < 5; ++i)
		{
			// each block of the day
			for (int j = 0; j < 8; ++j)
			{
				bool dont_bother = false;

				// going throguh each lecturer
				for (int l = 0; l < lects.size (); ++l)
				{
					// allocating lecturer to to current lecturer in the loop
					int lecturer = lects.at (l);

					// if one of the constraints are met, or if the course is already being taught by someone else
					if (!dont_bother && (l != 0) )
					{
						break;
					}
					// checking to see if there already exists a course block on that day
					for (int k = 0; k < course_times.size (); ++k)
					{
						int day = floor (course_times.at(k) / 8 ) ;
						// if wrong day, keep going through loop
						if (day != 5*i)
						{
							continue;
						}

						// if right day
						// if its the class before or after
						if ( abs (abs (course_times.at(k) - 8*day) - j)  == 1)
						{
							dont_bother = false;
						}
						// if theres a distance of one between the classes, then it can't be added
						else
						{
							dont_bother = true;
							break;
						}
					}

					// checking if the teacher has already taught two courses in a row
					if (!dont_bother)
					{
						int moment = j-1;
						if (moment > 0)
						{
							// previous block
							if (teachers.at (lecturer).at (8*i + moment) == 1)
							{
								-- moment ;
								if (moment >= 0)
								{
									// 2blocks ago (i.e. needs a break)
									if (teachers.at (lecturer).at (8*i + moment) == 1)
									{
										dont_bother = true;
									}
								}
							}
						}
					}

					// if I want / if I can to take that block 
					if ( !dont_bother && (ProblemUCS::LP.at (lecturer).at (8*i+j) == standards) && (teachers.at (lecturer).at (8*i+j) == -1) && (course_availabilities.at(8*i+j) < ProblemUCS::rooms ))
					{
						// setting the timetable to the lecturer, and incrementing all my constraint variables
						tt_line.at (8*i+j) = lecturer;
						teachers.at (lecturer).at (8*i+j) = 1;
						++ course_availabilities.at(8*i+j) ;
						course_times.push_back (8*i+j);

						// get out of the loops if the timetable shouldn't be modified anymore
						++ current_hours ;
						if (current_hours == ProblemUCS::cHours.at (course))
						{
							break;
						}
					}
				}

				// get out of the loops if we shouldn't modify the timetable anymore
				if (current_hours == ProblemUCS::cHours.at (course))
				{
					break;
				}
			}
			if (current_hours == ProblemUCS::cHours.at (course))
			{
				break;
			}
		}
		// decreasing the lecturer's standards or increasing the variable, if it can't all be done using 1's
		++ standards ;
		++ stop_hang ;
	}
	
	// finishing up the timetable modifications
	tt.at (course) = tt_line;
	return ;
}

int main (int argc, char const *argv[])
{
	// filter the input
	if(argc < 2) 
	{
		cerr << "usage: ./schedule must have problem attatched" << endl;
		exit(EXIT_FAILURE);
	}
	string input_question = argv[1];
	ProblemUCS::readUCSInstance (input_question) ;

	// filtering out the lunch times
	filter_lunch ();

	// making the timetable I plan to modify (initally all empty slots)
	IntMatrix timetable = create_IntMatrix(ProblemUCS::mC);

	// keeping track of what the lecturers have done
	IntMatrix lects_taken = create_IntMatrix(ProblemUCS::nL);

	// keeping track of what courses have done
	vector<int> rooms_at_a_time(40, 0);

	// parsing the timetable
	// ascending through the amount of the 1's inside one row
	int current_courses = 0 ;
	int counter = 0;
	int stop_hang = 0;
	while (current_courses < ProblemUCS::mC && stop_hang < 1000)
	{
		vector<int> lecturers;
		// looping through the courses in TL
		for (int i = 0; i < ProblemUCS::mC ; ++i)
		{
			// looping through the lecturers that can / cannot teach the course
			for (int j = 0 ; j < ProblemUCS::TL.at (i).size (); ++j)
			{
				// adding the lecturer that can teach the course to the lecturers vector
				if (ProblemUCS::TL.at (i).at (j) == 1)
				{
					lecturers.push_back (j) ;
				}
			}
			
			// want to filter the courses that can only be taught by one lecturer ascending by counters value
			// this shouldn't EVER happen 
			if ( (counter == 0) && (lecturers.size () == 0) )
			{
				cerr << "usage: There must be at least one lecturer able to do the course " << ProblemUCS::cNames.at (i) << endl;
				exit(EXIT_FAILURE);
			}

			// if the course can only be taught by one 
			else if ((counter == 1) && (lecturers.size () == 1))
			{
				parse_one (lecturers.at (0), i, timetable, lects_taken, rooms_at_a_time);
				++ current_courses;
			}

			// if the course can only be taught by two
			else if ((counter == 2) && (lecturers.size () == 2))
			{
				++ current_courses;
				parse_multiple (lecturers, i, timetable, lects_taken, rooms_at_a_time);
			}

			// if the course can only be taught by more than 2 lecturers
			else if ((counter > 2) && (lecturers.size () > 2))
			{
				++ current_courses;
				parse_multiple (lecturers, i, timetable, lects_taken, rooms_at_a_time);
			}
			lecturers.clear ();
		}
		++counter;
		++ stop_hang ;
	}

	// printing the vector to another file
	print_IntMatrix (timetable);

	// Printing the output becasue this part of the assignment is ambiguous
	cout << "Printing the timetable: \n";
	Solution::printTimetable (timetable, ProblemUCS::cNames, ProblemUCS::lNames);
	cout << "Checking constraints: \n";
	int f_ups = Solution::checkConstraints (timetable, ProblemUCS::rooms, ProblemUCS::cHours, ProblemUCS::LP, ProblemUCS::cNames, ProblemUCS::lNames ) << '\n';
	cout << "Fitness Value: \n";
	cout << Solution::getFitnessValue (timetable, ProblemUCS::mC, ProblemUCS::LP, ProblemUCS::cHours, f_ups) << '\n';
	return 0;
}