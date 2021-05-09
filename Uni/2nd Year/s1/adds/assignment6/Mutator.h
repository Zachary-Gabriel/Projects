#ifndef MUTATOR_H
#define MUTATOR_H

#include "Individual.h"

class Mutator{
public:
	// Modifying Individual so its child has different genes.
	virtual Individual* mutate(Individual*, int k) = 0;
};

#endif