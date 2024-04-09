#pragma once

#include <functional>

#include <matplot/matplot.h>


class Histogram {
private:
    double m_leftLimit, m_rightLimit;
    int m_numberOfBins;
    double m_binWidth;

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
    Histogram(const std::vector<double>& values, std::function<double(double)> pdf) :
        m_figure{ matplot::figure(true) }, m_axes{ m_figure->add_axes() }
    {
        int sampleSize = static_cast<int>(values.size());
        m_leftLimit = std::floor(*std::min_element(values.begin(), values.end()));
        m_rightLimit = std::floor(*std::max_element(values.begin(), values.end())) + 1;
        // TODO: Handle coefficient
        m_numberOfBins = 5 * static_cast<int>(std::floor(std::log(sampleSize)));
        m_binWidth = (m_rightLimit - m_leftLimit) / m_numberOfBins;
        
        m_binEdges.push_back(m_leftLimit);
        for (int i = 0; i < m_numberOfBins; i++) m_binEdges.push_back(m_binEdges.back() + m_binWidth);

        m_axes->hold(true);
        m_axes->xlim({ m_leftLimit, m_rightLimit });
        addHistogramPlot(values);
        addPlotPDF(pdf);
    }

    void show() {
        m_figure->show();
    }

    // TODO: Add save()
};
