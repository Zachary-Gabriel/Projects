#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <climits>

#include"EvalUCS.h"


using namespace std;

class Solution{
public:
	static vector<vector<int>>  Timetable;

	static bool readSolution (string fileName);

	static void printTimetable (const vector < vector <int>>& solution, const vector <string>& cNames, const vector <string>& lNames)  ;

	static int checkConstraints (vector < vector <int>>  solution, int rooms, vector <int> hoursperCourse, vector < vector <int>>  LP, vector <string> cNames, vector <string>  lNames);

	static double getFitnessValue (vector < vector <int>>  solution, int courses, vector < vector <int>> LP, vector <int> coursehours, int failedConstraints);

};

#endif