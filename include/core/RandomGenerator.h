#pragma once

#include <random>
#include <time.h>
#include <climits>

using namespace std;

// A simple random number generator that is used as a function object. 
// Returns a new random value every time operator() is called.

class RandomGenerator
{
public:

	RandomGenerator() : rng{rd()} {}

	//Returns a random positive integer value 
	unsigned long int operator()()
	{
		uniform_int_distribution<int> uni(0, INT_MAX);
		return uni(rng);
	}

	//Returns a random integer value between 0 and val
	unsigned long int operator()(int val)
	{
		uniform_int_distribution<int> uni(0, val);
		return uni(rng);
	}

	//Returns a random floating point value in the normal distribution specified by val and range
	float distribution(float val, float range = 0)
	{
		range = range < 0.0 ? -range : range; //convert negative numbers
		const unsigned vals = 100000;
		uniform_int_distribution<int> uni( static_cast<int>( val * vals - (range == 0 ? val / 2 * vals : range * vals) ), 
										   static_cast<int>( val * vals + (range == 0 ? val / 2 * vals : range * vals) )
		);
		return uni(rng) / static_cast<float>(vals);
	}

	//Generates a simple unique id
	static unsigned long int unique();

private:
	random_device rd;
	mt19937 rng;
	static unsigned long int unique_id;
};