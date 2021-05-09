#include <vector>
#include <string>
#include <algorithm>
class LotteryTicket
{
	std::string parse_two (int price, int b1, int b2)
	{
		if ( b1+ b2 == price ) return "POSSIBLE";
		if ( b1 == price ) return "POSSIBLE";
		if ( b2 == price ) return "POSSIBLE";
		return "IMPOSSIBLE" ;
	}

	std::string parse_three(int price, int b1, int b2, int b3)
	{
		if ( b1+ b2+ b3 == price ) return "POSSIBLE";
		
		if ( parse_two ( price, b2, b3 ) == "POSSIBLE") return "POSSIBLE" ;
		if ( parse_two ( price, b1, b3 ) == "POSSIBLE") return "POSSIBLE" ;
		if ( parse_two ( price, b1, b2 ) == "POSSIBLE") return "POSSIBLE" ;
		return "IMPOSSIBLE" ;
	}

	std::string parse_four (int price, int b1, int b2, int b3, int b4)
	{
		if ( b1+ b2+ b3 + b4 == price ) return "POSSIBLE";

		if ( parse_three ( price, b2, b3, b4 ) == "POSSIBLE") return "POSSIBLE" ;
		if ( parse_three ( price, b1, b3, b4 ) == "POSSIBLE") return "POSSIBLE" ;
		if ( parse_three ( price, b1, b2, b4 ) == "POSSIBLE") return "POSSIBLE" ;
		if ( parse_three ( price, b1, b2, b3 ) == "POSSIBLE") return "POSSIBLE" ;
		return "IMPOSSIBLE" ;
	}

public:
	std::string buy(int price, int b1, int b2, int b3, int b4)
	{
		if (parse_four (price, b1, b2, b3, b4) == "POSSIBLE") return "POSSIBLE" ;
		if (parse_four (price, b2, b3, b4, b1) == "POSSIBLE") return "POSSIBLE" ;
		if (parse_four (price, b3, b4, b1, b2) == "POSSIBLE") return "POSSIBLE" ;
		if (parse_four (price, b4 ,b1, b2, b3) == "POSSIBLE") return "POSSIBLE" ;
		return "IMPOSSIBLE";
	}

} ;