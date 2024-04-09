#pragma once

#include "View/Histogram.h"
#include "Model/ContinuousDistribution.h"


class Controller {
private:


public:
    Controller() {}


	void execute() {
		while (true) {
			std::vector<double> values;
			int experiments = 200;
			double lambda = 5;

			// TODO: Try upcasting
			std::unique_ptr<ContinuousDistribution<double>> distr = std::make_unique<ContinuousDistribution<double>>([&](double gamma) { return -std::log(1 - gamma) / lambda; }, 1488);

			for (int i = 0; i < experiments; i++) {
				values.push_back(distr->generateValue());
			}

			auto histRenderer = std::make_unique<Histogram>(values, [&](double x) { return lambda * std::exp(-lambda * x); });

			histRenderer->show();
		}

	}

};