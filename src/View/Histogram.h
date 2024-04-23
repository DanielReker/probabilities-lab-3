#pragma once

#include <functional>
#include <iomanip>
#include <format>

#include <matplot/matplot.h>


class Histogram {
private:
    double m_leftLimit, m_rightLimit;
    int m_numberOfBins;
    double m_binWidth;
    int m_sampleSize;

    std::vector<double> m_binEdges;
    std::vector<size_t> m_binCounts;

    matplot::figure_handle m_figure;
    matplot::axes_handle m_axes;

    void addPlotPDF(std::function<double(double)> pdf) {
        m_axes->fplot(pdf, { m_leftLimit, m_rightLimit })->line_width(1.5);
    }

    void addHistogramPlot(const std::vector<double>& values) {
        auto hist = m_axes->hist(values, m_binEdges);
        m_binCounts = hist->histogram_count(values, m_binEdges);
        hist->normalization(matplot::histogram::normalization::pdf);
    }


public:
    Histogram(const std::vector<double>& values, std::function<double(double)> pdf, const std::string& title, double binsCoefficient) :
        m_figure{ matplot::figure(true) }, m_axes{ m_figure->add_axes() }
    {
        m_sampleSize = static_cast<int>(values.size());
        m_leftLimit = std::floor(*std::min_element(values.begin(), values.end()));
        m_rightLimit = std::floor(*std::max_element(values.begin(), values.end())) + 1;
        m_numberOfBins = static_cast<int>(binsCoefficient * std::floor(std::log(m_sampleSize)));
        m_binWidth = (m_rightLimit - m_leftLimit) / m_numberOfBins;
        
        m_binEdges.push_back(m_leftLimit);
        for (int i = 0; i < m_numberOfBins; i++) m_binEdges.push_back(m_binEdges.back() + m_binWidth);

        m_axes->hold(true);
        m_axes->title(title);
        m_axes->xlim({ m_leftLimit, m_rightLimit });
        addHistogramPlot(values);
        addPlotPDF(pdf);
    }

    void show() {
        m_figure->show();
    }

    void save(const std::string& fileName) {
        m_figure->save(fileName);
    }

    void printTable(std::ostream& out, std::function<double(double)> CDF) {
        auto edgeIt = m_binEdges.begin();
        auto countIt = m_binCounts.begin();
        for (int bin = 0; bin < m_numberOfBins; bin++) {
            const double& from = *edgeIt;
            const double& to = *(edgeIt + 1);
            const size_t& count = *countIt;

            out << std::format("\\,[{:.9f}; {:.9f}) & {:.15f} & {:.15f}\\\\\n",
                from, to, static_cast<double>(count) / m_sampleSize, CDF(to) - CDF(from));

            edgeIt++;
            countIt++;
        }
    }
};
