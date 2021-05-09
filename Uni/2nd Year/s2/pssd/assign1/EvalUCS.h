#ifndef EVALUCS_H
#define EVALUCS_H

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

using namespace std;

const int maxsize=100000+5;
vector<string> split(const string& s, char delim);
vector<int> splitInt(const string& s, char delim);
template <typename T> bool parseField(string const& match, string input, T& output);

// template from Mike Seymour 
// (http://stackoverflow.com/questions/14590410/stoi-and-stoll-in-c)
template <typename T> T ston(string const & s) 
{
  stringstream ss(s);
  T result;
  ss >> result;  
  return result;
}

typedef vector<vector<int> > IntMatrix;

class ProblemUCS{
public:
  static int rooms;		  // number of teaching rooms
  static int mC;             // total number of courses to schedule
  static vector<int> cHours  ;     // hours per course
  static vector<string> cNames;    // Course names  
  static int  nL;                   // number of lecturers 
  static vector<string> lNames;    // Course names  
  static IntMatrix   TL;  // teacherr qualification (0 = no, 1 = yes)
  static IntMatrix   LP;  // 0 if no, 1 = fav, 2 = 2nd fav, 5 = least fav (rows = rating, cols = lNames)
  
  
  // function to read in UCS problem specification from a file
  static  bool readUCSInstance(string fileName);

  static string replaceGeneric(string s, 
    const string& search, 
    const string& replace);

  static string removeDoubleSpace(string s);  
};

// template to parse a input field (labelled with match)
// into an output reference type
// if there is no match then the output is left untouched.
// returns true if there is a match.
template <typename T> bool parseField(string const& match, 
string input, T& output){
  int pos;
  string stripped;
  bool res=false;
  if (pos=input.find(match)!=string::npos){
    res=true;
    input.replace(pos-1,match.length(),"");  // get rid of label
    stripped=ProblemUCS::removeDoubleSpace(input);     // strip to space
    output = ston<T>(stripped);               // convert to num
  }
  return res;
}

#endif