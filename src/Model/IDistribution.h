#pragma once

#include <concepts>


template<std::floating_point Float>
class IDistributuion {
public:
	virtual Float generateValue() = 0;
};