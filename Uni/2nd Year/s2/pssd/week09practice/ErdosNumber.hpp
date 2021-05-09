#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
class ErdosNumber
{
	class Yeet
	{
	public:
		std::string name;
		int length;
	} ;

	bool check_is_in (std::vector <Yeet> author, std::string tempName )
	{
		using namespace std;

		bool isIn = false;
		for (int i = 0; i < author.size () ; ++i)
		{
			Yeet tempYeet = author.at (i);
			
			if ( tempYeet.name == tempName ) isIn = true;
		}
		
		return isIn;
	}

public:

	std::vector <std::string> calculateNumbers(std::vector <std::string> publications)
	{
		using namespace std;
		vector <Yeet> author;
		vector <string> names;

		for ( int i = 0; i < publications.size () ; ++i )
		{
			string temp = publications.at (i), 
			tempName = "" ;

			for ( int j = 0; j < temp.size () ; ++j )
			{
				if ( temp[j] == ' ' )
				{
					if (true != check_is_in (author, tempName) )
					{
						Yeet tempYeet;
						tempYeet.name = tempName;

						if ( tempName == "ERDOS" )
						{
							tempYeet.length = 0;
						}

						else
						{
							tempYeet.length = 10000;
						}

						author.push_back (tempYeet) ;
					}
					tempName = "" ;
				}
				
				else
				{
					tempName += temp[j] ;
				}
			}

			if (true != check_is_in (author, tempName) )
			{
				Yeet tempYeet;
				tempYeet.name = tempName;

				if ( tempName == "ERDOS" )
				{
					tempYeet.length = 0;
				}

				else
				{
					tempYeet.length = 10000;
				}

				author.push_back (tempYeet) ;
			}
		}

		int count = 1,
		i = 0,
		size1 = publications.size (),
		size2 = author.size ();
		Yeet tempYeet ;
		vector<int> tempVec ;
		string tempName = "" ;

		while ( count < size2 && i < 1000000)
		{
			// finding the authors that are on this publications line
			string rowOfAuthors = publications.at(i % size1) ;
			tempName = "" ;
			for ( int j = 0; j < rowOfAuthors.size() ; ++j )
			{
				if ( rowOfAuthors[j] == ' ' )
				{
					for (int k = 0; k < size2; ++k) // adding the corresponding author indices to the list
					{
						tempYeet = author.at(k);
						if (tempName == tempYeet.name)
						{
							tempVec.push_back(k);
						}
					}
					tempName = "" ;
				}
				
				else
				{
					tempName += rowOfAuthors[j] ;
				}
			}
			
			// adding the last index to the index list
			for (int k = 0; k < size2; ++k)
			{
				tempYeet = author.at(k);
				if (tempName == tempYeet.name)
				{
					tempVec.push_back(k);
				}
			}

			// finding the smallest distance to erdos
			int min = 10000;
			for (int j = 0; j < tempVec.size() ; ++j)
			{
				int tempIndex = tempVec.at(j);
				tempYeet = author.at(tempIndex) ;
				if ( min > tempYeet.length )
				{
					min = tempYeet.length;
				}
			}

			// setting the smallest length to this
			if (min == 10000) ;
			else
			{
				for (int j = 0; j < tempVec.size() ; ++j)
				{
					int tempIndex = tempVec.at(j);
					tempYeet = author.at (tempIndex) ;
					if ( min != tempYeet.length )
					{
						if (tempYeet.length == 10000)
						{
							++count;
							author.at (tempIndex).length = min+1 ;
						}
						else
						{
							author.at (tempIndex).length = min+1 ;
						}
					}
				}
			}

			// iterating
			++i;
			tempVec.clear();
		}

		for (int i = 0; i < 2*size1; ++i)
		{
			string rowOfAuthors = publications.at(i % size1) ;
			tempName = "" ;
			for ( int j = 0; j < rowOfAuthors.size() ; ++j )
			{
				if ( rowOfAuthors[j] == ' ' )
				{
					for (int k = 0; k < size2; ++k) // adding the corresponding author indices to the list
					{
						tempYeet = author.at(k);
						if (tempName == tempYeet.name)
						{
							tempVec.push_back(k);
						}
					}
					tempName = "" ;
				}
				
				else
				{
					tempName += rowOfAuthors[j] ;
				}
			}
			
			// adding the last index to the index list
			for (int k = 0; k < size2; ++k)
			{
				tempYeet = author.at(k);
				if (tempName == tempYeet.name)
				{
					tempVec.push_back(k);
				}
			}

			// finding the smallest distance to erdos
			int min = 10000;
			for (int j = 0; j < tempVec.size() ; ++j)
			{
				int tempIndex = tempVec.at(j);
				tempYeet = author.at(tempIndex) ;
				if ( min > tempYeet.length )
				{
					min = tempYeet.length;
				}
			}

			// setting the smallest length to this
			if (min == 10000) ;
			else
			{
				for (int j = 0; j < tempVec.size() ; ++j)
				{
					int tempIndex = tempVec.at(j);
					tempYeet = author.at (tempIndex) ;
					if ( min != tempYeet.length )
					{
						if (tempYeet.length == 10000)
						{
							author.at (tempIndex).length = min+1 ;
						}
						else
						{
							author.at (tempIndex).length = min+1 ;
						}
					}
				}
			}

			// iterating
			tempVec.clear();

		}
		for (int i = 0; i < size2; ++i) // adding the number and the author together
		{
			Yeet tempYeet = author.at(i);
			string tempName = tempYeet.name;
			int tempLength = tempYeet.length;
			if (tempLength == 10000)
			{
				names.push_back( tempName ) ;
			}
			else
			{
				names.push_back( tempName + " " + to_string (tempLength) ) ;
			}
		}

		sort( names.begin(), names.end() ) ; // sorting the string alphabetically
		return names ;
	}
} ;