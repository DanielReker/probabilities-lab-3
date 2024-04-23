#pragma once


#include "View/Histogram.h"
#include "Model/ExponentialDistribution.h"
#include "Model/NormalDistribution.h"
#include "Model/ApproximatedNormalDistribution.h"

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
		const auto& params,
		Seed seed
	) {
		// TODO: Refactor dispatching
		if (name == "exponential") {
			return std::make_unique<ExponentialDistribution<double>>(seed, params["lambda"]);
		} else if (name == "normal") {
			return std::make_unique<NormalDistribution<double>>(seed, params["mu"], params["sigma"]);
		} else if (name == "normalApproximated") {
			return std::make_unique<ApproximatedNormalDistribution<double>>(
				seed, params["mu"], params["sigma"], params["n"]
			);
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

	std::string formatImageFilePath(
		const std::string& distribution,
		Seed seed,
		int sampleSize,
		const auto& params
	) {
		std::string imgFilePath = std::format(
			"img/{}_sampleSize{}_seed{}",
			distribution,
			sampleSize,
			seed
		);
		for (const auto& [paramName, paramValue] : params.items()) {
			imgFilePath += std::format("_{}{}", paramName, paramValue.get<double>());
		}
		imgFilePath += "/histogram.pdf";
		return imgFilePath;
	}

	std::string formatChartTitle(
		const std::string& distributionName,
		int sampleSize,
		const auto& params
	) {
		std::string chartTitle = std::format("{}, samples = {}", distributionName, sampleSize);
		for (const auto& [paramName, paramValue] : params.items()) {
			chartTitle += std::format(", {} = {}", paramName, paramValue.get<double>());
		}
		return chartTitle;
	}


public:
	Controller(const std::string& configFile) :
		m_config{ json::parse(std::ifstream(configFile)) } {
	}

	void execute() {
		std::string distributionName = m_config["distribution"];
		int sampleSize = m_config["sampleSize"];
		Seed seed = loadSeed();

		auto params = m_config["params"][distributionName];
		std::unique_ptr<IDistributuion<double>> distribution = getDistribution(distributionName, params, seed);

		auto sample = generateSample(distribution.get(), sampleSize);
		
		Histogram histRenderer(
			sample,
			[&](double x) { return distribution->probability(x); },
			formatChartTitle(
				m_config["distributions"][distributionName]["name"].get<std::string>(),
				sampleSize,
				params
			),
			m_config["binsCoefficient"]
		);

		histRenderer.printTable(std::cout, [&](double x) { return distribution->cumulativeProbability(x); });
		histRenderer.save(formatImageFilePath(distributionName, seed, sampleSize, params));
		histRenderer.show();
	}
};