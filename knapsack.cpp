#include "knapsack.h"
#include <iostream>
#include <algorithm>

void Knapsack::load(std::istream& in) {
    in >> n >> capacity;
    value.resize(n);
    weight.resize(n);
    for (int i = 0; i < n; ++i) {
        in >> value[i] >> weight[i];
    }
}

double Knapsack::evaluate(const std::vector<int>& solution) const {
    double total = 0.0;
    for (int i = 0; i < n; ++i) {
        if (solution[i] == 1) total += value[i];
    }
    return total;
}

bool Knapsack::isFeasible(const std::vector<int>& solution) const {
    double totalWeight = 0.0;
    for (int i = 0; i < n; ++i) {
        if (solution[i] == 1) totalWeight += weight[i];
    }
    return totalWeight <= capacity + 1e-9;
}

std::vector<int> Knapsack::randomSolution() {
    std::vector<int> sol(n, 0);
    static std::mt19937 gen(888);
    std::uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < n; ++i) {
        sol[i] = dist(gen);
    }

    if (!isFeasible(sol)) {
        std::vector<int> indices(n);
        for (int i = 0; i < n; ++i) indices[i] = i;
        std::sort(indices.begin(), indices.end(), [&](int a, int b) {
            return value[a]/weight[a] < value[b]/weight[b];
        });
        for (int idx : indices) {
            if (sol[idx] == 1) {
                sol[idx] = 0;
                if (isFeasible(sol)) break;
            }
        }
    }
    return sol;
}

void Knapsack::printSolution(const std::vector<int>& solution, double val) const {
    double totalWeight = 0.0;
    for (int i = 0; i < n; ++i) {
        if (solution[i] == 1) totalWeight += weight[i];
    }
    std::cout << totalWeight << "\n";
    for (int i = 0; i < n; ++i) std::cout << solution[i];
    std::cout << "\n";
}

std::vector<int> Knapsack::Search2opt(const std::vector<int>& start) const {
    std::vector<int> best = start;
    double bestVal = evaluate(best);
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 0; i < n; ++i) {
            std::vector<int> candidate = best;
            candidate[i] = 1 - candidate[i];
            if (isFeasible(candidate)) {
                double candVal = evaluate(candidate);
                if (candVal > bestVal + 1e-9) {
                    best = candidate;
                    bestVal = candVal;
                    improved = true;
                }
            }
        }
    }
    return best;
}

std::vector<int> Knapsack::Search3opt(const std::vector<int>& start) const {
    std::vector<int> best = start;
    double bestVal = evaluate(best);
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                std::vector<int> candidate = best;
                candidate[i] = 1 - candidate[i];
                candidate[j] = 1 - candidate[j];
                if (isFeasible(candidate)) {
                    double candVal = evaluate(candidate);
                    if (candVal > bestVal + 1e-9) {
                        best = candidate;
                        bestVal = candVal;
                        improved = true;
                        break;
                    }
                }
            }
            if (improved) break;
        }
    }
    return best;
}