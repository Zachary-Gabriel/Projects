#ifndef BITFLIPPROB_H
#define BITFLIPPROB_H

#include "Mutator.h"

class BitFlipProb: public Mutator{
public:
	// Using probability to randomly switch up the ones and zeros
	Individual* mutate(Individual*, int k);
};

#endif