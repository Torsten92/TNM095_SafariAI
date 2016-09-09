#pragma once

#include <random>
#include <climits>

// A simple random number generator that is used as a function object. 
// Returns a new random value every time operator() is called.
class RandomGenerator
{
public:

	//Returns a random positive integer value 
	unsigned long int operator()()
	{
		uniform_int_distribution<int> distribution(0,INT_MAX);
		return distribution(generator);
	}

	//Returns a random integer value between 0 and val
	unsigned long int operator()(int val)
	{
		uniform_int_distribution<int> distribution(0,val);
		return distribution(generator);
	}

private:
	default_random_engine generator;

};