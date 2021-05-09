#include"Solution.h"

vector<vector<int> >  Solution::Timetable;

bool Solution::readSolution(string fileName){

    //  file format is Timetable.csv 
    string input;   // input string to hold line
    ifstream sr(fileName.c_str());   // open the file

    if (! sr.is_open()) return false;  // file can't open then exit
 
    int courses = ProblemUCS::mC;

    //first, resize Timetable to be  mC x 40  
    Timetable.resize(courses, vector<int>(40, -1)); 

    bool status;
    // read the allocation matrix, one row at at time 
    
    stringstream ss;
  try{
    for (int i=0;i< courses;i++)  {
       //cout << "line" << i << endl;
       status = getline(sr,input)?true:false;     // file can open grab the first line
       ss.str(input);
       ss.clear();  
       int j = 0;
       string next;
       while (getline(ss, next, ',' )) { 
             Timetable[i][j] = stoi(next);
             j++;
         }
         if (j != 40)  
             cout << "line " << i << " size is " << j << " instead of 40"  << endl;
         
     }

    cout << "Timetable read "  << endl;
    /*
    for (int i=0;i<courses;i++)  {
      for (int j=0;j<40;j++)  
            cout <<  Timetable[i][j]  << " " ;
      cout << endl;
    } */

  }catch(int e){
      // exception in processing return false
      printf(" exception caught \n" );
      return false;
       }

    return true;

    }


void Solution::printTimetable(const vector < vector <int>>&  solution, const vector <string>& cNames, const vector <string>&  lNames) {
   vector < string> weekdays = {"             Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
   vector < string> labelHours = {" 9-10", "10-11", "11-12","Lunch", "13-14", "14-15", "15-16","16-17"};//8 working hours 
 
   vector < vector <int> > copysol(solution);
   int courses = cNames.size();
   int index=0;
   int hoursWeek=40;
   cout << "-----------------------------------------------------------------------------------------------" << endl;
   for (int i = 0; i < weekdays.size(); i++) {
            cout << weekdays[i] << "          ";
        }
   cout << endl;
   cout << "-----------------------------------------------------------------------------------------------" << endl;
   for (int i = 0; i < labelHours.size(); i++) {
     // iterate over solution for first course for each day 
    bool found ;    
    bool next = true;
    while (next) {
        next = false;
        cout << labelHours[i];
        for (int j = i; j < hoursWeek ; j+=8) {
        found = false;
         for (int k = 0; k < courses; k++) {
            if(copysol[k][j]!=-1) {
                 if (!found)  {
                      found  = true;
                      cout <<  "  " << std::setw (4) <<  cNames[k] << "("  << std::left  << std::setw (6) << lNames[copysol[k][j]] << ")    " ;  
                      copysol[k][j] = -1;          
                      }
                 else {
                       next = true;
                       break;
                  }
              }
        }
         if (!found)
                 cout << "                  ";   
        }
        cout << endl;
     }
        cout << "-----------------------------------------------------------------------------------------------" << endl;
    }
  }




  int Solution::checkConstraints(vector < vector <int>>  solution, int rooms, vector <int> hoursperCourse, vector < vector <int>>  LP, vector <string> cNames, vector <string>  lNames) {
        //violated constraint 
        //1. No classes allocted at lunch break 12-1 (done)
        //2. A lecture can not have a separated session in the same day (done)
        //3. No more than two hours per lecture in the same day (done)
        //4. at the same time no more lectures than rooms (done)
        //5. Don't arrange a lecture when a lecturer is unavailabe (done)
        //6. NO lecture can teach more than two consecutive periods without break
        //
        int hoursWeek=40;
        int il,il2;//indice the lecturer
        int failedConstraints=0;

        //Based on the solution we fill busyRooms to verified if is not allocated at the 
        //same hour more than one course //always be the same 
       
        vector <int>  busyRooms(hoursWeek,0);

        int courses = cNames.size();
        int nl = lNames.size();
        int day=5;

        //To know how many hours per day a Lecturer works
         vector < vector <int>>   lectDay;
         lectDay.resize(nl, vector<int>(day,0));
        

        int hoursday=0;
        for (int i = 0; i < courses; i++) {
            day=0;
            for (int j = 0; j < hoursWeek; j++) {
                il=solution[i][j];//indice lecturer 
                if(il!=-1){
                    // check lecturer is allocated to course 
                    if (ProblemUCS::TL[i][il] != 1) {
                       cout << "Major Violation: lecturer " << lNames[il] <<  " is not allocated to course" << cNames[i] << endl;
                        return 100000;
                    }
                    //Lunch constraint  positions 3,11,19,27,35
                    if(j%8 == 3) {
                        cout << "Constraint Violation: class  for lecturer" << lNames[il] <<  " is allocated at lunch break" << endl;
                        failedConstraints++;
                    }                    
                    
                    busyRooms[j]++;
                    //more classes allocated at the same time  than the number of rooms 
                    if(busyRooms[j]>rooms){
                        cout << "Constraint Violation: More classes than rooms at the same hour, day " << day <<  "hour " << j%8 << endl;
                        // printf("Constraint Violation: More classes than rooms at the same hour, day %d, hour %d \n", day, j%8 );
                        failedConstraints++;
                    }
                    
                    //sum the hour in the lecturers array 
                    //Check that the lecturer is available in his preferences 
                    if(LP[il][j]==0) { //means the lecturer isn't available    
                        cout << "Constraint Violation: lecturer" << lNames[il] <<  " was assigned to course " << cNames[i] <<  " in a busy slot "  << endl;
                        // printf("Constraint Violation:  lecturer %s was assigned to course %s in a busy slot \n", lNames[il], cNames[i]);
                        //failedConstraints++;                        
                    }
                    
                    lectDay[il][day]++;//increase the hour 
                        //cout << " lecturer" << lNames[il] <<  " teaching hour "  << j << " in day " << day << endl;
                        //cout << " lecturer hours " << lectDay[il][day] << endl;
                    //Verify if the lecturer took a rest at least one hour 
                    if(lectDay[il][day]>2){
                        il2=solution[i][j-1];
                        if(il2 !=-1 && il2==il)//the lecturer is teaching more than two hours without at least one hour break
                        {
                            cout<<"day "<<day<<" hour "<<j%8<<endl;
                            cout << "Constraint Violation: lecturer" << lNames[il] <<  " is teaching more than two hours without break"  << endl;
                            // printf("Constraint Violation: lecturer %s is teaching more than two hours without break \n", lNames[i]);
                            failedConstraints++;
                        }
                    }
                        
                    //We added 1 hour per this course 
                    hoursday++;
                    if(hoursday>2){
                        cout << "Constraint Violation: More than two hours was assigned to course " << cNames[i] <<  " on day "   << day << endl;
                        //printf("Constraint Violation: More than two hours for course %s in  day %d \n", cNames[i], day);
                    failedConstraints++; 
                    }                
                    else if(hoursday>1){//No separated hours 
                        if(solution[i][j-1]==-1){
                            cout << "Constraint Violation: two  separate session for course " << cNames[i] <<  " on day "   << day << endl;
                            //printf("Constraint Violation: two separate session for course %s on day %d \n", cNames[i], day);
                            failedConstraints++; 
                        }
                    }
                    
                }// j != -1 
                
                //check when is a new day 
                if(j%8==7){
                    //a new day is starting 
                    day++;
                    //A lecture with more than two hours per day                          
                    hoursday=0;
                    //re-star hoursday 
                }
                    
            }//end for j-week
        }//end for i- courses 
        return failedConstraints;
    }


    double Solution::getFitnessValue( vector < vector <int>>  solution, int courses, vector < vector <int>> LP, vector <int> coursehours,
                                    int failedConstraints) {
        
        int hoursWeek=40;
        int il;//index of lecturer allocated to it 
        double penalization;
        //double
        //i is rows 
        int totalAllocateHours=0;

        double sum=0;
        for (int i = 0; i < courses; i++) {
            //j is cols 
            for (int j = 0; j < hoursWeek; j++) {
                if(solution[i][j]!=-1){
                    totalAllocateHours++;
                    il=solution[i][j];
                    //sum the preference
                    if (LP[il][j] == 0)
                       sum = sum+ 10;    //  busy slot is penalised by 10
                     else 
                       sum=sum+(double)LP[il][j];
                    
                }
                
            }
            
        }//end for courses 


        //we're penalise per each constraint that the solution violated and the number of times 
        penalization=failedConstraints*20;

        int totalhours = accumulate(coursehours.begin(), coursehours.end(), 0);  

        if(totalAllocateHours!=totalhours){
            printf("You didn't allocate all the hours or allocate more than the hours");
            //We penalize for any discrepancy 
            penalization = 15*(double)abs(totalAllocateHours-totalhours);
            sum = sum+ penalization;
            
        }
        //we get the average per allocated hours
        sum=sum+failedConstraints;
        return sum/(double)totalhours;
    }