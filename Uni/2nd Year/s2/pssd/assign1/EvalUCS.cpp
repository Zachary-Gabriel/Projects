#include"EvalUCS.h"

int ProblemUCS::rooms;		  // number of teaching rooms
int ProblemUCS::mC;             // total number of courses to schedule
vector<int> ProblemUCS::cHours  ;     // hours per course
vector<string> ProblemUCS::cNames;    // Course names  
int ProblemUCS::nL;                   // number of lecturers 
vector<string> ProblemUCS::lNames;    // Course names  
IntMatrix ProblemUCS::TL;
IntMatrix ProblemUCS::LP;

bool ProblemUCS::readUCSInstance(string fileName){
    string input;
    ifstream sr(fileName.c_str());
    if (! sr.is_open()) return false;

    bool status=getline(sr,input)?true:false;

        // match on keyword read in number, assign to var and continue;
        if(parseField<int>("Rooms",input,rooms)){
    cout << "rooms: " << rooms  ;
        }else{
        // there was a problem reading... report and return
        cerr << "inconsistent file - first line should have Rooms" << endl;
        return false;
        }

        // read second line
        status=getline(sr,input)?true:false;

        if(parseField<int>("Courses",input,mC)){
    cout << " mCourses: " << mC << endl;
    ;
        }else{
        // there was a problem reading... report and return
        cerr << "inconsistent file - second line should have course number" << endl;
        return false;
        }


    // now we get the line with the course hours
    status=getline(sr,input)?true:false;  // read the line

    string hours, next; 
    hours = replaceGeneric(input, "Hours", "");

    // used for breaking words 
    stringstream ss;
    ss << hours;

    while (getline(ss, next, ',' )) { 
        cHours.push_back(stoi(next)); 
    }

    cout << "hours read," ;
     for (int i=0;i<mC;i++) {
          cout << cHours[i] << " ";
    }
     cout << endl;

    // now we get the line with the course names
    status=getline(sr,input)?true:false;  // read the line

    ss.clear(); 
    ss.str(input);
    ss >> next;

    while (getline(ss, next, ',' )) { 
        cNames.push_back(removeDoubleSpace(next)); 
    }

    cout << " course names read" << endl;
    for (int i=0;i<mC;i++) {
       cout << cNames[i] <<  endl;
    }
    cout<<endl;
        // read lecture numbers 
        status=getline(sr,input)?true:false;

        if(parseField<int>("Lecturers",input,nL)){
    cout << "lecturers: " << nL ;
    ;
        }else{
        // there was a problem reading... report and return
        cerr << "inconsistent file -  line should have lecturer number" << endl;
        return false;
        }

    // now we get the line with lecturers names
    status=getline(sr,input)?true:false;  // read the line

    ss.clear(); 
    ss.str(input);

    while (getline(ss, next, ',' )) { 
        lNames.push_back(removeDoubleSpace(next)); 
    }

    cout << " lecture names read" << endl;
    // for (int i=0;i<nL;i++) 
    //cout << lNames[i] <<  endl;


    status=getline(sr,input)?true:false;  // read the  comment line for TL 
    //first, resize TL to be mC x nL 
    TL.resize(mC, vector<int>(nL, 0)); 

    // read the TL matrix, one row at at time 
    for (int i=0;i<mC;i++)  {
            status=getline(sr,input)?true:false;  // read the line
            ss.clear();  
            ss.str(input);
            int j = 0;
            while (getline(ss, next, ',' )) { 
                TL[i][j] = stoi(next);
                j++;          
            }
    }

    status=getline(sr,input)?true:false;  // read the  comment line for LP  
    //first, resize LP to be  nL x 40  
    LP.resize(nL, vector<int>(40, 0)); 

    // read the LP matrix, one row at at time 
    for (int i=0;i<nL;i++)  {
            status=getline(sr,input)?true:false;  // read the line
            ss.clear();  
            ss.str(input);
            int j = 0;
            while (getline(ss, next, ',' )) { 
                LP[i][j] = stoi(next);
                j++;
            }
        if (j != 40)
            cout << "Warning: line size is" << j << " instead of 40" << endl; 
        }

    cout << "LP  read" << endl;
     for (int i=0;i<nL;i++) {
       for (int j=0;j<40;j++) 
           cout << LP[i][j] <<  " ";
       cout << endl;
     }

    return true;
    }

string ProblemUCS::replaceGeneric(string s, 
                const string& search, 
                const string& replace){
    int pos;
    int len;
    string result=s;
    len=search.length();
    pos=result.find(search);
    while(pos!=string::npos){
        result.replace(pos,len,replace);
        pos=result.find(search);
    }
    return result;
}

string ProblemUCS::removeDoubleSpace(string s){
    string res =replaceGeneric(s,"  "," ");
    return replaceGeneric(res," ","");
}