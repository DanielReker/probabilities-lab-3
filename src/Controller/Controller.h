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
			auto CDF = [&](double x) { return 1 - std::exp(-lambda * x); };
			auto inverseCDF = [&](double gamma) { return -std::log(gamma) / lambda; };
			return std::make_unique<ContinuousDistribution<double>>(inverseCDF, PDF, CDF, seed);
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

	Seed loadSeed() {
		Seed seed = m_config["seed"];
		if (seed == 0) seed = std::random_device{}();
		return seed;
	}


public:
	Controller(const std::string& configFile) :
		m_config{ json::parse(std::ifstream(configFile)) } {}

	void execute() {
		std::string distributionName = m_config["distribution"];
		std::map<std::string, double> params =
			m_config["params"][distributionName].get<std::map<std::string, double>>();
		int sampleSize = m_config["sampleSize"];
		Seed seed = loadSeed();

		std::unique_ptr<IDistributuion<double>> distribution = getDistribution(distributionName, params, seed);
		auto sample = generateSample(distribution.get(), sampleSize);


		std::string histogramTitle = std::format("{}, samples = {}", distributionName, sampleSize);
		for (const auto& [paramName, paramValue] : params) {
			histogramTitle += std::format(", {} = {}", paramName, paramValue);
		}
		Histogram histRenderer(sample, [&](double x) { return distribution->probability(x); }, histogramTitle, m_config["binsCoefficient"]);

		std::string imgFilePath = std::format("img/{}_sampleSize{}_seed{}", distributionName, sampleSize, seed);
		for (const auto& [paramName, paramValue] : params) {
			imgFilePath += std::format("_{}{}", paramName, paramValue);
		}
		imgFilePath += "/histogram.pdf";

		histRenderer.printTable(std::cout, [&](double x) { return distribution->cumulativeProbability(x); });
		histRenderer.save(imgFilePath);
		histRenderer.show();
	}
};