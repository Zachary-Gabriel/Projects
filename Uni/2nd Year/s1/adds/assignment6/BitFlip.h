#ifndef BITFLIP_H
#define BITFLIP_H

#include "Mutator.h"

class BitFlip: public Mutator{
public:
	// Swaps the k'th element of the Individual from one to zero or zero to one
	Individual* mutate(Individual*, int k);
};

#endif