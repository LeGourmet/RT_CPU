#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__

#include <random>
#include <time.h>

namespace RT_CPU
{
	// Return a pseudo random float between 0 and 1
	static std::mt19937							 gen((unsigned int)time(0));
	static std::uniform_real_distribution<float> dis(0.f, 1.f);
	static inline float randomFloat() { return dis(gen); }
}

#endif