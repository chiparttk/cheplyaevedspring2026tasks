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

    struct Item {
        double v, w, ratio;
        int idx;
    };

    void dfs(int lvl, double cur_V, double cut_W, const std::vector<Item>& items, double& maxProfit, std::vector <int>& bestSolution,std::vector <int>& curSolution ) const;

    void repairChromosome(std::vector<int>& chromosome) const;
    std::vector<int> tournamentSelection(int populationSize, const std::vector<std::vector<int>>& population, const std::vector<double>& fitness, std::mt19937& gen) const;
public:
    void load(std::istream& in) override;
    double evaluate(const std::vector<int>& solution) const override;
    bool isFeasible(const std::vector<int>& solution) const override;
    std::vector<int> randomSolution() override;
    void printSolution(const std::vector<int>& solution, double value) const override;

    std::vector<int> Search2opt(const std::vector<int>& start) const;
    std::vector<int> Search3opt(const std::vector<int>& start) const;
    std::vector<int> BranchAndBound() const;
    std::vector<int> Genetic(int populationSize, int generations, double mutationRate) const;
};

#endif