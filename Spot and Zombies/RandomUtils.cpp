/*
**A simple library of random numbers routines 
---------------------------------------------
** Seed() - seed the random number generator from current system time 
** Random(max) - produce a random number in range [1..max]
*/

#include "RandomUtils.h"

// -------------
// seeds the random number generator from current system time 
// must be called once (at the beginning of the program) 
// so that the numbers generated by the Random function are different every time
void Seed() {
	srand(static_cast<unsigned>(time(0))); 
}

// -------------
// produces a random number in range [1..max]
// max must be positive
int Random(int max) {
	assert(max > 0);
	return (rand() % max) + 1;
}

