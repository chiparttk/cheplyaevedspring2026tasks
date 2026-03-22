#ifndef TSP_H
#define TSP_H

#include "task.h"
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

class TSP : public Task {
private:
    int vertexs;
    std::vector<double> x, y;

    double distance(int i, int j) const {
        double dx = x[i] - x[j];
        double dy = y[i] - y[j];
        return std::sqrt(dx*dx + dy*dy);
    }

public:
    void load(std::istream& in) override;
    double evaluate(const std::vector<int>& solution) const override;
    bool isFeasible(const std::vector<int>& solution) const override;
    std::vector<int> randomSolution() override;
    void printSolution(const std::vector<int>& solution, double value) const override;

    std::vector<int> Search2opt(const std::vector<int>& start) const;
    std::vector<int> Search3opt(const std::vector<int>& start) const;
};

#endif