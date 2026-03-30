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
    std::cout << "Value : " << val <<", Weight : " << totalWeight << "\n";
    // for (int i = 0; i < n; ++i) std::cout << solution[i];
    // std::cout << "\n";
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

void Knapsack::dfs(int level, double cur_V, double cur_W, const std::vector<Item>& items, 
double& maxProfit, std::vector <int>& bestSolution,std::vector <int>& curSolution) const {
    if (cur_V > maxProfit) {
        maxProfit = cur_V;
        bestSolution = curSolution;
    }

    if (level == n) return;

    if (cur_W + items[level].w <= capacity) {
        curSolution[items[level].idx] = 1;
        dfs(level + 1, cur_W + items[level].w, cur_V + items[level].v,
            items, maxProfit, bestSolution, curSolution);
        curSolution[items[level].idx] = 0;
    }
    
    double bound = cur_V;
    double total_w = cur_W;
    int i = level + 1;

    while (i < n && total_w + items[i].w < capacity) {
        total_w += items[i].w;
        bound += items[i].v;
        i++;
    }
    if (i < n) {
        bound += (capacity - total_w) * items[i].ratio;
    }

    if (bound > maxProfit) {
        dfs(level + 1, cur_V, cur_W, items, maxProfit, bestSolution, curSolution);
    }
}

std::vector<int> Knapsack::BranchAndBound() const {
    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        items[i] = {value[i], weight[i], value[i]/weight[i], i};
    }
    std::sort(items.begin(), items.end(), [](const  Item& a, const  Item& b){return a.ratio > b.ratio;});

    double maxProfit = .0;
    std::vector<int> bestSolution(n,0);
    std::vector<int> curSolution(n, 0);

    dfs(0, .0, .0, items, maxProfit, bestSolution, curSolution);

    return bestSolution;
}

void Knapsack::repairChromosome(std::vector<int>& chromosome) const {
    double totalWeight = .0;
    for (int i = 0; i < n; ++i) {
        if (chromosome[i] == 1) totalWeight += weight[i];
    }

    if (totalWeight <= capacity) return;

    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i) indices[i] = i;
    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        return (value[a] / weight[a]) < (value[b] / weight[b]);
    });

    // while (totalWeight > capacity) {
    //     int worst_idx = -1;
    //     double worst_ratio = 1e9;
    //     for (int i = 0; i < n; ++i) {
    //         if (chromosome[i] == 1 && (value[i]/weight[i] < worst_ratio)) {
    //             worst_ratio = value[i]/weight[i];
    //             worst_idx = i;
    //         }
    //     }
    //     if (worst_idx != -1) {
    //         chromosome[worst_idx] = 0;
    //         totalWeight -= weight[worst_idx];
    //     }
    // }

    for (int idx : indices) {
        if (totalWeight > capacity && chromosome[idx] == 1) {
            chromosome[idx] = 0;
            totalWeight -= weight[idx];
        }
    }
}

std::vector<int> Knapsack::tournamentSelection(int populationSize, const std::vector<std::vector<int>>& population, const std::vector<double>& fitness, std::mt19937& gen) const {
    int tournamentSize = 3;
    std::uniform_int_distribution<int> pop_dist(0, populationSize - 1);

    int best_idx = pop_dist(gen);

    for (int k = 1; k < tournamentSize; ++k) {
        int idx = pop_dist(gen);
        if (fitness[idx] > fitness[best_idx]) {
            best_idx = idx;
        }
    }
    return population[best_idx];
}

std::vector<int> Knapsack::Genetic(int populationSize, int generations, double mutationRate) const {
    std::mt19937 gen(888);
    std::uniform_real_distribution<double> real_dist(.0, 1.);

    std::vector<std::vector<int>> population(populationSize, std::vector<int>(n, 0));

    for (int i = 0; i < populationSize; ++i) {
        for (int j = 0; j < n; ++j) {
            if (real_dist(gen) > .5) population[i][j] = 1;
        }
        repairChromosome(population[i]);
    }

    std::vector<int> bestGlobalSolution(n, 0);
    double bestGlobalFitness = -1.;

    for (int gen_idx = 0; gen_idx < generations; ++gen_idx) {
        std::vector<double> fitness(populationSize);
        double currentBestFit = -1.;
        int currentBestIdx = -1;
        //оцениваем
        for (int i = 0; i < populationSize; ++i) {
            fitness[i] = evaluate(population[i]);
            if (fitness[i] > currentBestFit) {
                currentBestFit =  fitness[i];
                currentBestIdx = i;
            }
        }

        if (currentBestFit > bestGlobalFitness) {
            bestGlobalFitness = currentBestFit;
            bestGlobalSolution = population[currentBestIdx];
        }

        std::vector<std::vector<int>> newPopulation(populationSize);

        newPopulation[0]  = population[currentBestIdx];
        //скрещиваем
        for (int i = 1; i < populationSize; ++i) {

            std::vector<int> parent1 = tournamentSelection(populationSize, population, fitness, gen);
            std::vector<int> parent2 = tournamentSelection(populationSize, population, fitness, gen);
            std::vector<int> child(n);

            for (int j = 0; j < n; ++j) {
                child[j] = (real_dist(gen)  > .5) ? parent1[j] : parent2[j];
            }
            //мутация
            for (int j = 0; j < n; ++j) {
                if (real_dist(gen) < mutationRate) {
                    child[j] = 1 - child[j];
                }
            }

            repairChromosome(child);
            newPopulation[i] = child;
        }

        population = newPopulation;

    }
    return  bestGlobalSolution;
}