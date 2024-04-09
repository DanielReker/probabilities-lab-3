#pragma once


#include "View/Histogram.h"
#include "Model/ContinuousDistribution.h"

#include <map>
#include <exception>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

using Seed = unsigned long long;


class Controller {
private:
	json m_config;

	std::unique_ptr<IDistributuion<double>> getDistribution(
		const std::string& name,
		const std::map<std::string, double>& params,
		Seed seed
	) {
		if (name == "exponential") {
			const double& lambda = params.at("lambda");
			auto PDF = [&](double x) { return lambda * std::exp(-lambda * x); };
			std::function<double(double)> inverseCDF = [&](double gamma) { return -std::log(1 - gamma) / lambda; };
			return std::make_unique<ContinuousDistribution<double>>(inverseCDF, PDF, seed);
		} else throw std::invalid_argument("Unknown distribution");
		
	}

	std::vector<double> generateSample(IDistributuion<double>* distribution, int size) {
		std::vector<double> sample;
		for (int i = 0; i < size; i++) {
			double value = distribution->generateValue();
			sample.push_back(value);
		}
		return sample;
	}


public:
	Controller(const std::string& configFile) :
		m_config{ json::parse(std::ifstream(configFile)) } {}

	void execute() {
		std::string distributionName = m_config["distribution"];
		std::map<std::string, double> params =
			m_config["params"][distributionName].get<std::map<std::string, double>>();
		int sampleSize = m_config["sampleSize"];
		Seed seed = m_config["seed"];

		std::unique_ptr<IDistributuion<double>> distribution = getDistribution(distributionName, params, seed);
		auto sample = generateSample(distribution.get(), sampleSize);

		Histogram histRenderer(sample, [&](double x) { return distribution->probability(x); } );

		histRenderer.show();
	}
};