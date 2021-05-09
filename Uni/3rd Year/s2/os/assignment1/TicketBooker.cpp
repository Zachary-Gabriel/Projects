/*
created by: Boon Aun Lim (Moses), Zachary Gabriel
ID: a1757792, a1724849
time: 8/09/2020
Contact Email: a1757792@student.adelaide.edu.au, a1724849@student.adelaide.edu.au
Include int main(int argc,char *argv[])
input: argv[1]
output: Screen

input sample:
ID arrival_time priority age total_tickets_required
for example: s1 3 1 0 50

output sample:
ID, arrival and termination times, ready time and durations of running and waiting
*/
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct Ticket
{
	// input variables
	string ID = "b16b00b5";
	int arrival_time = 0xb16b00b5;
	int priority = 0xb16b00b5;
	int age = 0xb16b00b5;
	int tickets = 0xb16b00b5;
	
	// output variables
	/* name
	   arrival_time
	*/
	int end = 0xb16b00b5;
	int ready = -1;
	int running = 0xb16b00b5;
	int waiting = 0xb16b00b5;
};

// global variables
static int size_Ticket = 10;
static int verbose = 3; // 3 == no dubugging, 2 == output debugging, 1 == all debugging
int start_q2 = 0;

// prints the output to the console
void output(vector<Ticket> output) {
	//debugging
	if (verbose < 2)
		cout << "calling 'output'\n";
	
	cout << "name   arrival   end   ready   running   waiting" << endl;
	for (int i = 0; i < output.size(); i++) {
		// for debugging
		if (verbose < 3)
		{
			cout << "input ID: ";
			cout << output.at(i).ID << "\t arrival_time: ";
			cout << output.at(i).arrival_time << "\tpriority: ";
			cout << output.at(i).priority << "\tage: ";
			cout << output.at (i).age << "\ttickets: ";
			cout << output.at (i).tickets << endl << "output: ";			
		}
		cout << output.at(i).ID << "\t\t";
		cout << output.at(i).arrival_time << "\t";
		cout << output.at(i).end << "\t\t";
		cout << output.at(i).ready << "\t\t";
		cout << output.at(i).running << "\t\t";
		cout << output.at(i).waiting << endl;		
	}

	//debugging
	if (verbose < 2)
		cout << "leaving 'output'\n";
	
	return ;
}

// sorts the vectors in ascending order according to their priority number
vector<vector<string>> sort(vector<vector<string>> queue, int index) {
	// debugging
	if (verbose < 2)
		cout << "calling sort, with index: "<< index << endl;
	
	const int column = index;	
	if (index == 0)
	{
		// sorting using std::sort()
		sort (queue.begin(), queue.end(), [&column](const vector<string> &item1, const vector<string> &item2) 
		{
			return (item1[column]) < (item2[column]);
		});
		return queue;
	}

	// sorting using std::sort()
	sort (queue.begin(), queue.end(), [&column](const vector<string> &item1, const vector<string> &item2) 
	{
		return stoi(item1[column]) < stoi(item2[column]);
	});

	// debugging
	if (verbose < 2)
		cout << "leaving sort"<< endl;
	
	return queue;
}


bool sorting_function (const Ticket &item1, const Ticket &item2)
{
	if (item1.tickets == item2.tickets)
	{
		if ((item1.priority == item2.priority))
		{
			if (item1.arrival_time == item2.arrival_time)
			{
				return item1.ID < item2.ID;
			}
			return item1.arrival_time < item2.arrival_time;
		}
		return (item1.priority < item2.priority);
	}
	return item1.tickets < item2.tickets;
}

void sort (vector<Ticket>* queue)
{
	// debugging
	if (verbose < 2)
		cout << "calling sort"<< endl;
	const int column = 0;

	// sorting using std::sort() + start_q2
	sort (queue-> begin() , queue->end(), sorting_function);

	// debugging
	if (verbose < 2)
		cout << "leaving sort"<< endl;
	return ;	
}

// updates the time and priority of the elements in the queues
void promotion_check (vector<Ticket>* queue1, vector<Ticket>* queue2, int time)
{
	if (verbose < 2)
		cout << "entering promotion_check" << endl;
	
	// incrementing the wait time for all those not getting proccessed
	for (int i = 0; i < queue2->size (); ++i)
	{
		if (time > queue2->at(i).arrival_time)
		{
			queue2->at(i).age += 5;
			if ( queue2->at(i).age == 100 )
			{
				// debugging
				if (verbose < 3)
					cout << "promoting: " << queue2->at(i).ID << endl;
				
				// upgrading
				queue2->at (i).priority--;
				queue2->at (i).age = 0;
				
				// if valid promotion, remove from queue2 and add to queue1
				if (queue2->at (i).priority < 4)
				{
					//hunting index applicable in queue1

					queue1->push_back (queue2-> at (i));											
					queue2->erase (queue2-> begin()+i);
					i--;
				}
				else
				{
					if (verbose < 3)
					{
						cout << "stuck in elo hell\n";
					}
				}
			}

		}
	}
	if (verbose < 2)
		cout << "leaving promotion_check" << endl;
	
	return ;
}

// fetches ticket quota
int reset_queue1 (vector<Ticket>* queue1, vector<Ticket>* queue2, vector<Ticket>* done, int time, int* index)
{
	if (verbose < 2)
		cout << "resetting queue1\n";
	int quota = -6;
	
	if (queue1->size () != 0)
	{
		if (*index == -2)
		{
			// do nothing
		}
		else if (queue1->at (*index).tickets == 0)
		{
			// popping into done
			queue1->at(*index).end = time;
			done-> push_back(queue1-> at(*index));
			queue1-> erase(queue1-> begin() + *index);
		}
		else if (queue1->at (*index).tickets != 0)
		{
			queue1-> at (*index).age ++;

			if ((queue1-> at (*index).age) % 2 == 0) {
				queue1-> at (*index).age = 0;
				queue1-> at (*index).priority ++;
			}

			if (queue1-> at(*index).priority > 3) { // supposed to go to other queue
				//demote by pushing to front of q2
				queue1-> at(*index).age = 0;
				queue2-> insert (queue2-> begin(), queue1-> at(*index));
				queue1-> erase(queue1-> begin()+*index);
				// start_q2 ++;
			}
			else // supposed to go to back of queue
			{
				// push to back of queue 1
				queue1-> push_back(queue1-> at(*index));
				queue1-> erase(queue1-> begin()+*index);
			}
		}

		// resetting index to nothing value
		*index = -2;
		// check if any other elements are able to be worked on in queue1
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < queue1->size (); ++j)
			{
				if (queue1->at(j).arrival_time <= time && queue1->at(j).priority == i)
				{
					quota = (((10 - queue1->at(j).priority) * 10) / 5);
					*index = j;
					return quota;
				}
			}
		}
	}
	
	if (verbose < 2)
		cout << "leaving reset queue1\n";

	return quota;
}

// parses the second queue
void secondQueue(vector<Ticket>* queue1, vector<Ticket>* queue2, vector<Ticket>* output, int time) 
{
	int index = 0;
	int length = queue2-> size ();
	
	//debugging
	if (verbose < 2)
		cout << "calling second queue, time:" << time << endl;
	
	// if the ticket can be proccessed
	while (index < length)
	{
		if (queue2->at (index).tickets > 0 && queue2->at (index).arrival_time <= time)
		{
			queue2-> at (index).tickets --;
			// debugging
			if (verbose < 3)
			{
				cout << queue2->at(index).ID << "'s tickets now at: " << queue2->at(index).tickets << endl;
			}
			
			queue2-> at (index).age = 0; // resetting age (equivalent of age = 0)
			if (queue2-> at (index).ready == -1)
			{
				// debugging
				if (verbose < 3)
				{
					cout << "setting " << queue2->at(index).ID << " 'ready' to: " << time << " in q1\n";
				}
				queue2-> at (index).ready = time;
			}

			// if this ticket was the last
			if (queue2-> at(index).tickets == 0)
			{
				queue2-> at (index).end = time+5;
				output-> push_back (queue2-> at(index));
				if (verbose < 3)
				{
					cout << queue2-> at(index).ID << " pushed to ouput at time: " << time<< endl;
				}
				
				// if (index < start_q2) 
				// 	start_q2--;
				queue2-> erase (queue2-> begin() + index);
				
				if (queue2-> size () == 0)
				{
					//debugging
					if (verbose < 2)
						cout << "leaving second queue"<< endl;
					return ;
				}
			}
			if (verbose < 2)
				cout << "leaving second queue"<< endl;	
			return ;
		}
		index++;
	}

	//debugging
	if (verbose < 2)
	{
		cout << "skipping time" << endl;
	}
	return ;
}

// parses the first queue
void firstQueue(vector<Ticket>* queue1, vector<Ticket>* queue2, vector<Ticket>* done, int time, int ticketQuota, int index) 
{
	// debugging
	if (verbose < 2)
	{
		cout << "entering first queue, time: " << time << ", quota: " << ticketQuota << ", index: " << index << endl;
		if (index != -2)
			cout << "ID" << queue1->at(index).ID << endl;
	}
	
	if (index == -2)
	{
		// if no ops can be done in q1, go to q2
		if (queue2-> size() != 0)
		{
			// debugging
			if (verbose < 2)
				cout << "q1 needs to wait. entering q2" << endl;
			secondQueue (queue1, queue2, done, time);
		}
		else
		{
			// debugging
			if (verbose < 2)
				cout << "waiting for time to pass" << endl;
		}

		//debugging
		if (verbose < 2)
			cout << "leaving first queue\n";
		return;
	}


	if (ticketQuota > 0 && queue1->at(index).tickets > 0 && queue1->at(index).arrival_time <= time)
	{
		queue1-> at(index).tickets --;
		if (verbose < 3)
		{
			cout << queue1->at(index).ID << "'s tickets now at: " << queue1->at(index).tickets << endl;
		}
		if (queue1->at(index).ready == -1)
		{
			// debugging
			if (verbose < 3)
			{
				cout << "setting " << queue1->at(index).ID << " 'ready' to: " << time <<" in q1\n";
			}
			
			queue1->at(index).ready = time;
		}
	}
	else if (queue1->at(index).arrival_time > time)
	{
		// if no ops can be done in q1, go to q2
		if (queue2-> size() != 0)
		{
			// debugging
			if (verbose < 2)
				cout << "q1 needs to wait. entering q2" << endl;
			secondQueue (queue1, queue2, done, time);
		}
		else
		{
			// debugging
			if (verbose < 3)
				cout << "waiting for time to pass" << endl;
		}
	}
	else
	{
		if (verbose < 3)
			cout << "something went wrong" << endl;
	}

	//debugging
	if (verbose < 2)
		cout << "leaving first queue\n";
	return ;
}


// executes the queue
void start (vector<Ticket>* queue1, vector<Ticket>* queue2, vector<Ticket>* output)
{
	//for debugging
	if (verbose < 2)
		cout << "calling start\n";
	
	int time = 0;
	int index = -2;
	int quota = 0;

	// sorting initial queue2. plan to push queue1 elements to the front after this initial sort	
	while (queue1->size () != 0 || queue2->size () != 0)
	{
		// debugging
		if (verbose < 3)

			cout << "queue1 size: " << queue1->size() << ", queue2 size: " << queue2->size () << ", time: " << time << ", index: " << index << endl;
		
		// updates the queues so that any time related changes occur after every iteration
		promotion_check (queue1, queue2, time);
		
		if (!queue2->empty())
		{
			sort (queue2);
		}

		if (queue1->size () != 0)
		{
			// when either the time limit has expired or the tickets have been processed, reset queue1
			if (index == -2)
			{
				quota = reset_queue1 (queue1, queue2, output, time, &index);
				if (!queue2->empty())
				{
					sort (queue2);
				}
			}

			else if (quota == 0 || queue1-> at(index).tickets == 0)
			{
				quota = reset_queue1 (queue1, queue2, output, time, &index);
				if (!queue2->empty())
				{
					sort (queue2);
				}
			}
			
			// if more than 1 element is in q1 after changes
			if (queue1-> size () != 0 && index != -2)
			{
				firstQueue (queue1, queue2, output, time, quota, index);
			}
			// if removing the last element of q1
			else if (queue2-> size () != 0)
			{
				secondQueue (queue1, queue2, output, time);
			}
		}
		// run queue2 iff queue1 is empty.
		else
		{
			secondQueue (queue1, queue2, output, time);
		}

		time += 5;
		quota --;
	}

	//for debugging
	if (verbose < 3)
	{
		cout << "end time: " << time-5 << endl;
		if (verbose < 2)
			cout << "leaving start\n";
	}
	return ;
}

// converts vector<string> into Ticket object
vector<Ticket> convert (vector<vector<string>> double_vec)
{
	if (verbose < 2)
		cout << "calling convert" << endl;

	vector<Ticket> ret;
	for (int i =0; i < double_vec.size (); ++i)
	{
		if (verbose < 3)
			cout << "converting " << double_vec.at(i).at(0) << endl;

		Ticket temp;
		temp.ID = double_vec.at(i).at(0);
		temp.arrival_time = stoi(double_vec.at(i).at(1));
		temp.priority = stoi(double_vec.at(i).at(2));
		temp.age = stoi(double_vec.at(i).at(3));
		temp.tickets = stoi(double_vec.at(i).at(4));
		ret.push_back (temp);
	}

	if (verbose < 2)
		cout << "leaving convert" << endl;
	
	return ret;
}

// calcuates the execution time
void running_calc (vector<Ticket>* queue)
{
	if (verbose < 2)
		cout << "calling running_calc\n";
	
	for (int i =0; i < queue->size (); ++i)
	{
		queue->at(i).running = 5*(queue->at(i).tickets);
	}

	if (verbose < 2)
		cout << "leaving running_calc\n";
	return ;
}

// calcuates the waiting time
void waiting_calc (vector<Ticket>* queue)
{
	if (verbose < 2)
		cout << "calling waiting_calc\n";

	for (int i = 0; i < queue->size (); ++i)
	{
		queue-> at(i).waiting = queue->at(i).end - queue->at(i).running - queue->at(i).ready;
	}
	
	if (verbose < 2)
		cout << "leaving waiting_calc\n";
	return ;
}

// Converts the argc input text into a double vector
vector<vector<string>> inputParser(char* input) {
	if (verbose < 2) // debugging
		cout << "calling 'input parser'\n";
	
	ifstream file(input);
	string line;
	vector<vector<string>> parseInput;
	
	while(getline(file, line)) {
		vector<string> lineData;
		stringstream ss(line);
		string input;

		while (ss >> input) {
			lineData.push_back(input);
		}

		parseInput.push_back(lineData);
	}
	
	if (verbose < 2) // debugging
		cout << "leaving 'input parser'\n";
	
	return parseInput;
}

int main(int argc, char** argv) {
	// customer id[0], arrival time[1], priority[2], age[3], total tickets required[4]
	vector<vector<string>> bookTickets = inputParser(argv[1]);
	vector<vector<string>> queue1, queue2 ;
	vector<Ticket> queue1_obj, queue2_obj, output_vec;

	// splits the customers into 2 queues based on their priority number
	for (int i = 0; i < bookTickets.size(); i++) {
		if (stoi(bookTickets[i][2]) < 4) {
			queue1.push_back(bookTickets[i]);
		} else {
			queue2.push_back(bookTickets[i]);
		}
	}

	// sorting queue 1	
	queue1 = sort (queue1, 0);
	queue1 = sort (queue1, 1);	// arrival time
	queue1 = sort (queue1, 2);

	// sorting queue 2
	queue2 = sort (queue2, 0);
	queue1 = sort (queue1, 1);	// arrival time
	queue2 = sort (queue2, 2);
	queue2 = sort (queue2, 4);

	// converting the sorted vectors into obejcts	
	queue1_obj = convert(queue1);
	queue2_obj = convert(queue2);

	// calculating the running time
	running_calc (&queue1_obj);
	running_calc (&queue2_obj);

	// starts the program
	start (&queue1_obj, &queue2_obj, &output_vec);

	// calculating the waiting time
	waiting_calc (&output_vec);

	// couts the output
	output(output_vec);
	return 0;	
}