#pragma once

#include "NormalDistribution.h"


template<std::floating_point Float>
class ApproximatedNormalDistribution : public NormalDistribution<Float> {
private:
	int m_sampleSize;


public:
	ApproximatedNormalDistribution(Seed seed, Float mu, Float sigma, int sampleSize) :
		NormalDistribution<Float>{ seed, mu, sigma }, m_sampleSize{ sampleSize } { }

	Float generateValue() override {
		Float sum = 0;
		for (int i = 0; i < m_sampleSize; i++) {
			sum += this->generateUniformRandom();
		}
		Float standardDistributionValue = sum;

		// Centering
		standardDistributionValue -= static_cast<Float>(m_sampleSize) / 2;
		// Normalization
		standardDistributionValue /= std::sqrt(static_cast<Float>(m_sampleSize) / 12);

		return this->m_sigma * standardDistributionValue + this->m_mu;
	}
};