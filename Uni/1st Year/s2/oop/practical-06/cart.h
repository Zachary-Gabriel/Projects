#ifndef cart_h
#define cart_h

#include "meerkat.h"

class cart 
{
public:
	int volume;
	meerkat * animals;
	
	cart();
	bool addMeerkat(meerkat cat);
	void emptyCart();
	void printMeerkats();
	~cart();
};

#endif