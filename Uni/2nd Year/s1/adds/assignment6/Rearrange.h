#ifndef REARRANGE_H
#define REARRANGE_H

#include "Mutator.h"

class Rearrange: public Mutator{
public:
	// Changes the Individual by putting the first up to k elements at the back of the string.
	Individual* mutate(Individual*, int k);
};

#endif