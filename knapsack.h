#ifndef KNAPSACK_H
#define KNAPSACK_H

#include "task.h"
#include <vector>
#include <random>

class Knapsack : public Task {
private:
    int n;
    double capacity;
    std::vector<double> value, weight;

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