#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

class BridgeSort
{
public:
	std::string sortedHand(std::string hand)
	{
		using namespace std;
		vector<int> c_ints;
		vector<int> d_ints;
		vector<int> h_ints;
		vector<int> s_ints;

		vector<char> c_chars;
		vector<char> d_chars;
		vector<char> h_chars;
		vector<char> s_chars;

		string results = "";

		for (int i = 0; i < hand.size (); ++i)
		{
			if (hand[i] == 'C')
			{
				++i;
				int intof = int (hand[i]) - 48 ;
				if (0 <= intof && intof <= 9)
				{
					c_ints.push_back (intof) ;
				}
				else
				{
					c_chars.push_back (hand[i]) ;
				}
			}
			if (hand[i] == 'D')
			{
				++i;
				int intof = int (hand[i]) - 48 ;
				if (0 <= intof && intof <= 9)
				{
					d_ints.push_back (intof) ;
				}
				else
				{
					d_chars.push_back (hand[i]) ;
				}
			}
			if (hand[i] == 'H')
			{
				++i;
				int intof = int (hand[i]) - 48 ;
				if (0 <= intof && intof <= 9)
				{
					h_ints.push_back (intof) ;
				}
				else
				{
					h_chars.push_back (hand[i]) ;
				}
			}
			if (hand[i] == 'S')
			{
				++i;
				int intof = int (hand[i]) - 48 ;
				if (0 <= intof && intof <= 9)
				{
					s_ints.push_back (intof) ;
				}
				else
				{
					s_chars.push_back (hand[i]) ;
				}
			}
		}

		sort (c_ints.begin(), c_ints.end()) ;
		sort (d_ints.begin(), d_ints.end()) ;
		sort (h_ints.begin(), h_ints.end()) ;
		sort (s_ints.begin(), s_ints.end()) ;
		
		// clubs
		for (int i = 0; i < c_ints.size (); ++i)
		{
			results += "C" + to_string ( c_ints.at (i) ) ;
		}

		for (int i = 0; i < c_chars.size (); ++i)
		{
			if (c_chars.at (i) == 'T')
			{
				results += "CT" ;
			}
		}
		for (int i = 0; i < c_chars.size (); ++i)
		{
			if (c_chars.at (i) == 'J')
			{
				results += "CJ" ;
			}
		}
		for (int i = 0; i < c_chars.size (); ++i)
		{
			if (c_chars.at (i) == 'Q')
			{
				results += "CQ" ;
			}
		}
		for (int i = 0; i < c_chars.size (); ++i)
		{
			if (c_chars.at (i) == 'K')
			{
				results += "CK" ;
			}
		}
		for (int i = 0; i < c_chars.size (); ++i)
		{
			if (c_chars.at (i) == 'A')
			{
				results += "CA" ;
			}
		}

		// diamonds
		for (int i = 0; i < d_ints.size (); ++i)
		{
			results += "D" + to_string ( d_ints.at (i) ) ;
		}

		for (int i = 0; i < d_chars.size (); ++i)
		{
			if (d_chars.at (i) == 'T')
			{
				results += "DT" ;
			}
		}
		for (int i = 0; i < d_chars.size (); ++i)
		{
			if (d_chars.at (i) == 'J')
			{
				results += "DJ" ;
			}
		}
		for (int i = 0; i < d_chars.size (); ++i)
		{
			if (d_chars.at (i) == 'Q')
			{
				results += "DQ" ;
			}
		}
		for (int i = 0; i < d_chars.size (); ++i)
		{
			if (d_chars.at (i) == 'K')
			{
				results += "DK" ;
			}
		}
		for (int i = 0; i < d_chars.size (); ++i)
		{
			if (d_chars.at (i) == 'A')
			{
				results += "DA" ;
			}
		}

		// hearts
		for (int i = 0; i < h_ints.size (); ++i)
		{
			results += "H" + to_string ( h_ints.at (i) ) ;
		}

		for (int i = 0; i < h_chars.size (); ++i)
		{
			if (h_chars.at (i) == 'T')
			{
				results += "HT" ;
			}
		}
		for (int i = 0; i < h_chars.size (); ++i)
		{
			if (h_chars.at (i) == 'J')
			{
				results += "HJ" ;
			}
		}
		for (int i = 0; i < h_chars.size (); ++i)
		{
			if (h_chars.at (i) == 'Q')
			{
				results += "HQ" ;
			}
		}
		for (int i = 0; i < h_chars.size (); ++i)
		{
			if (h_chars.at (i) == 'K')
			{
				results += "HK";
			}
		}
		for (int i = 0; i < h_chars.size (); ++i)
		{
			if (h_chars.at (i) == 'A')
			{
				results += "HA" ;
			}
		}

		// spades
		for (int i = 0; i < s_ints.size (); ++i)
		{
			results += "S" + to_string ( s_ints.at (i) ) ;
		}

		for (int i = 0; i < s_chars.size (); ++i)
		{
			if (s_chars.at (i) == 'T')
			{
				results += "ST" ;
			}
		}
		for (int i = 0; i < s_chars.size (); ++i)
		{
			if (s_chars.at (i) == 'J')
			{
				results += "SJ" ;
			}
		}
		for (int i = 0; i < s_chars.size (); ++i)
		{
			if (s_chars.at (i) == 'Q')
			{
				results += "SQ";
			}
		}
		for (int i = 0; i < s_chars.size (); ++i)
		{
			if (s_chars.at (i) == 'K')
			{
				results += "SK" ;
			}
		}
		for (int i = 0; i < s_chars.size (); ++i)
		{
			if (s_chars.at (i) == 'A')
			{
				results += "SA" ;
			}
		}

		return results ;
	}
};