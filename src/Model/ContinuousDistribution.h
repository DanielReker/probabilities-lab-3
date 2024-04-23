#pragma once

#include <functional>
#include <random>

#include "IDistribution.h"

using Seed = unsigned long long;

template<std::floating_point Float>
class ContinuousDistribution : public IDistributuion<Float> {
private:
	std::mt19937_64 m_randomGenerator;
	std::uniform_real_distribution<Float> m_uniformDistribution;

protected:
	ContinuousDistribution(Seed seed) :
		m_randomGenerator{ seed }, m_uniformDistribution{ 0, 1 } {
	}

	// Generates uniformly distributed random float in range [0, 1]
	Float generateUniformRandom() {
		return m_uniformDistribution(m_randomGenerator);
	}
};