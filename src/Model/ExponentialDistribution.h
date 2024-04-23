#pragma once

#include "ContinuousDistribution.h"


template<std::floating_point Float>
class ExponentialDistribution : public ContinuousDistribution<Float> {
private:
	Float m_lambda;
	

public:
	ExponentialDistribution(Seed seed, Float lambda) :
		ContinuousDistribution<Float>{ seed }, m_lambda{ lambda } { }

	Float generateValue() override {
		Float gamma = this->generateUniformRandom();
		return -std::log(gamma) / m_lambda;
	}

	Float probability(Float value) override {
		return m_lambda * std::exp(-m_lambda * value);
	}

	Float cumulativeProbability(Float value) override {
		return 1 - std::exp(-m_lambda * value);
	}
};