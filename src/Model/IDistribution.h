#pragma once

#include <concepts>


template<std::floating_point Float>
class IDistributuion {
public:
	virtual Float generateValue() = 0;
	virtual Float probability(Float value) = 0;
	virtual Float cumulativeProbability(Float value) = 0;
};