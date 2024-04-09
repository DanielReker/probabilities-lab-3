#pragma once

#include <functional>
#include <random>

#include "IDistribution.h"

template<std::floating_point Float>
class ContinuousDistribution : IDistributuion<Float> {
	using Function = std::function<Float(Float)>;
	using Seed = unsigned long long;

private:
	Function m_inverseCDF;

	std::mt19937_64 m_randomGenerator;
	std::uniform_real_distribution<Float> m_uniformDistribution;

public:
	ContinuousDistribution(Function inverseCDF, Seed seed) :
		m_inverseCDF{ inverseCDF }, m_randomGenerator{ seed }, m_uniformDistribution{ 0, 1 } { }

	Float generateValue() override {
		return m_inverseCDF(m_uniformDistribution(m_randomGenerator));
	}
};