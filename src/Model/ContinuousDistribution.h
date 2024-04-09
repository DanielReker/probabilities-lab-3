#pragma once

#include <functional>
#include <random>

#include "IDistribution.h"

template<std::floating_point Float>
class ContinuousDistribution : public IDistributuion<Float> {
	using Function = std::function<Float(Float)>;
	using Seed = unsigned long long;

protected:
	Function m_inverseCDF;
	Function m_PDF;

	std::mt19937_64 m_randomGenerator;
	std::uniform_real_distribution<Float> m_uniformDistribution;

public:
	ContinuousDistribution(Function inverseCDF, Function PDF, Seed seed) :
		m_inverseCDF{ inverseCDF }, m_PDF{ PDF },
		m_randomGenerator { seed }, m_uniformDistribution{ 0, 1 } { }

	Float generateValue() override {
		double value = m_inverseCDF(m_uniformDistribution(m_randomGenerator));
		return value;
	}

	Float probability(Float value) override {
		return m_PDF(value);
	}
};