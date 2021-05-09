#include "Queue.h"


#include<iostream>
using namespace std;

int main(){
    
    Queue s;
    int tmp;
    
    // these operations can't help
    cout << s.remove() << " " << s.remove() << endl;
    
    // now add a few times
    s.add(1);
    s.add(4);
    s.add(3);
    s.add(2);
    
    // now remove a few times .
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    
    // now add a few times - last ones won't work
    s.add(5);
    s.add(6);
    s.add(7);
    s.add(8);
    s.add(17);
    s.add(18);
    s.add(27);
    s.add(28);

    // now remove a few more times - last few will be -1's.
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;
    cout << s.remove() << endl;

    
}
