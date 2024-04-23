#pragma once

#include "ContinuousDistribution.h"

#include <numbers>


template<std::floating_point Float>
class NormalDistribution : public ContinuousDistribution<Float> {
private:
	Float m_mu, m_sigma;


public:
	// Normal distribution PDF with mu = 0, sigma = 1
	static Float PDF(Float value) {
		return std::exp(-value * value / 2) / std::sqrt(2 * std::numbers::pi);
	}

	// Normal distributuion PDF
	static Float PDF(Float value, Float mu, Float sigma) {
		return PDF((value - mu) / sigma) / sigma;
	}

	// Normal distribution CDF with mu = 0, sigma = 1
	static Float CDF(Float value) {
		return (std::erf(value / std::sqrt(2)) + 1) / 2;
	}

	// Normal distributuion CDF
	static Float CDF(Float value, Float mu, Float sigma) {
		return CDF((value - mu) / sigma);
	}


	NormalDistribution(Seed seed, Float mu, Float sigma) :
		ContinuousDistribution<Float>{ seed }, m_mu{ mu }, m_sigma{ sigma } { }

	Float generateValue() override {
		Float gamma = this->generateUniformRandom();

		Float left = -1e9, right = 1e9;
		while (right - left > 1e-12) {
			Float mid = (right + left) / 2;
			if (cumulativeProbability(mid) - gamma <= 0) left = mid;
			else right = mid;
		}
		return left;
	}

	Float probability(Float value) override { return PDF(value, m_mu, m_sigma); }

	Float cumulativeProbability(Float value) override { return CDF(value, m_mu, m_sigma); }
};