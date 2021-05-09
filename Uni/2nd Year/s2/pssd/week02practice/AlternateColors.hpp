#include <string>

class AlternateColors
{
public:
	std::string getColor(long r, long g, long b, long k)
	{
	// three colours
		if ((r > 0 && g > 0) && b > 0){
		// all equal 
			if (r == g && g == b){
				if (k%3 == 1 || k%3 == -2)
				{
					return "RED";
				}
				if (k%3 == 2 || k%3 == -1)
				{
					return "GREEN";
				}
				if (k%3 == 0)
				{
					return "BLUE";
				}
			}

		// less green
			if(r >= g && b >= g){
				b -= g;
				r -= g;
				k = k - 3*g;
				g -= g;
				if(k <= 0){
					if (k%3 == 1 || k%3 == -2)
					{
						return "RED";
					}
					if (k%3 == 2 || k%3 == -1)
					{
						return "GREEN";
					}
					if (k%3 == 0)
					{
						return "BLUE";
					}
				}
				return getColor(r,g,b,k);
			}
		// less blue
			if(r >= b && g >= b){
				g -= b;
				r -= b;
				k -= 3*b;
				b -= b;
				if(k <= 0){
					if (k%3 == 1 || k%3 == -2)
					{
						return "RED";
					}
					if (k%3 == 2 || k%3 == -1)
					{
						return "GREEN";
					}
					if (k%3 == 0)
					{
						return "BLUE";
					}
				}
				return getColor(r,g,b,k);
			}
		// less red
			if(b >= r && g >= r){
				g -= r;
				b -= r;
				k -= 3*k;
				r -= r;
				if(k <= 0){
					if (k%3 == 1 || k%3 == -2)
					{
						return "RED";
					}
					if (k%3 == 2 || k%3 == -1)
					{
						return "GREEN";
					}
					if (k%3 == 0)
					{
						return "BLUE";
					}
				}
				return getColor(r,g,b,k);
			}
		}
	// two colours
	// blue = 0
		if (r > 0 && g > 0 && b == 0)
		{
			if (r == g) {
				if (k%2 == 0)
				{
					return "GREEN";
				}
				if (k%2 == -1 || k%2 == 1)
				{
					return "RED";
				}
			}
			if (r < g){
				g-=r;
				k-=2*r;
				r-=r;
				if(k <= 0){
					if (k%2 == 0)
					{
						return "GREEN";
					}
					if (k%2 == -1 || k%2 == 1)
					{
						return "RED";
					}
				}
				return "GREEN";
			}
			if (r > g){
				r-=g;
				k-=2*g;
				g-=g;
				if(k <= 0){
					if (k%2 == 0)
					{
						return "GREEN";
					}
					if (k%2 == -1 || k%2 == 1)
					{
						return "RED";
					}
				}
				return "RED";
			}
		}

	//green = 0
		if (r > 0 && g == 0 && b > 0)
		{
			if (r == b) {
				if (k%2 == 0)
				{
					return "BLUE";
				}
				if (k%2 == -1 || k%2 == 1)
				{
					return "RED";
				}
			}
			if (r < b){
				b-=r;
				k-=2*r;
				r-=r;
				if(k <= 0){
					if (k%2 == 0)
					{
						return "BLUE";
					}
					if (k%2 == -1 || k%2 == 1)
					{
						return "RED";
					}
				}
				return "BLUE";
			}
			if (r > b){
				r-=b;
				k-=2*b;
				b-=b;
				if(k <= 0){
					if (k%2 == 0)
					{
						return "BLUE";
					}
					if (k%2 == -1 || k%2 == 1)
					{
						return "RED";
					}
				}
				return "RED";
			}
		//red = 0
			if (r == 0 && g > 0 && b > 0)
			{
				if (g == b) {
					if (k%2 == 0)
					{
						return "BLUE";
					}
					if (k%2 == -1 || k%2 == 1)
					{
						return "GREEN";
					}
				}
				if (g < b){
					b-=g;
					k-=2*g;
					g-=g;
					if(k <= 0){
						if (k%2 == 0)
						{
							return "BLUE";
						}
						if (k%2 == -1 || k%2 == 1)
						{
							return "GREEN";
						}
					}
					return "BLUE";
				}
				if (g > b){
					g-=b;
					k-=2*b;
					b-=b;
					if(k <= 0){
						if (k%2 == 0)
						{
							return "BLUE";
						}
						if (k%2 == -1 || k%2 == 1)
						{
							return "GREEN";
						}
					}
					return "GREEN";
				}
			}
		}
	// one colour 
		else if (r == 0 && b == 0 && g > 0)
		{
			return "GREEN";
		}
		else if (r == 0 && b > 0 && g == 0)
		{
			return "BLUE";
		}
		else if (r > 0 && b == 0 && g == 0)
		{
			return "RED";
		}
		else {
			return "oops";
		}
	}
};