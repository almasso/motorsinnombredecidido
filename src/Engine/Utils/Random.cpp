#include "Random.h"

std::default_random_engine Random::_generator;

float Random::generate(float t) {
	return generateRange(0.0f, t);
}

int Random::generate(int t) {
	return generateRange(0, t);
}

float Random::generateRange(float a, float b) {
	std::uniform_real_distribution uni(a, b);
	return uni(_generator);
}

int Random::generateRange(int a, int b) {
	std::uniform_int_distribution uni(a, b);
	return uni(_generator);
}