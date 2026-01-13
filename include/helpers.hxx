#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <functional>
#include <random>

#include "types.hxx"

using ProbabilityGenerator = std::function<double()>;

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern PropabilityGenerator probability_generator;

#endif /* HELPERS_HPP_ */
