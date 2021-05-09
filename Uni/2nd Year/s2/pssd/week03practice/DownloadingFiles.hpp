#include <string>
#include <vector>

class DownloadingFiles{
public:
	double actualTime(std::vector<std::string> tasks)
	{
	// Preallocating memory
		std::vector<int> speed; 
		std::vector<int> time;
		double runTime = 0;
		int runningSpeed = 0;
		int length = tasks.size();
		double bestTime = 0;
		int bestIndex = 0;
		int i = 0;
		// Converting the string into two int vectors
		for (i = 0; i < length; i++)
		{
			int var = (tasks[i].find(' '));
			speed.push_back(std::stoi(tasks[i].substr(0,var)));
			time.push_back(std::stoi(tasks[i].substr(var+1,tasks[i].size())));
		}

		// reducing the size of the two arrays by one, whilst calculating the the run time of the download
		while (speed.size() > 0)
		{
			bestTime = 10001;	// via constraints
			length = speed.size();

			for(i = 0; i < length; i++)
			{
				if (time[i]/(double(speed[i])) < bestTime)
				{
					bestTime = time[i]/double(speed[i] + runningSpeed);
					bestIndex = i;
				}
			}
			runTime += (time[bestIndex]-runTime) * speed[bestIndex] / (runningSpeed + speed[bestIndex]);
			runningSpeed += speed[bestIndex];
			speed.erase(speed.begin()+bestIndex);
			time.erase(time.begin()+bestIndex);
		}
		return runTime;
	}
};
