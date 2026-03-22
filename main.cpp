#include <iostream>
#include "tsp.h"
#include "knapsack.h"
#include "solver.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <task_type>\n";
        std::cerr << "   task_type: 1 for TSP, 2 for Knapsack\n";
        return 1;
    }
    int taskType = std::atoi(argv[1]);
    Solver solver;

    if (taskType == 1) {
        TSP tsp;
        tsp.load(std::cin);
        std::cout << "2-opt:\n";
        auto sol2opt = solver.solve(&tsp, [&](const std::vector<int>& start) {
            return tsp.Search2opt(start);
        });
        double len2opt = tsp.evaluate(sol2opt);
        tsp.printSolution(sol2opt, len2opt);
        std::cout << "\n3-opt:\n";
        auto sol3opt = solver.solve(&tsp, [&](const std::vector<int>& start) {
            return tsp.Search3opt(start);
        });
        double len3opt = tsp.evaluate(sol3opt);
        tsp.printSolution(sol3opt, len3opt);
    } else if (taskType == 2) {
        Knapsack knap;
        knap.load(std::cin);
        std::cout << "2-opt:\n";
        auto sol2opt = solver.solve(&knap, [&](const std::vector<int>& start) {
            return knap.Search2opt(start);
        });
        double val2opt = knap.evaluate(sol2opt);
        knap.printSolution(sol2opt, val2opt);
        std::cout << "\n3-opt:\n";
        auto sol3opt = solver.solve(&knap, [&](const std::vector<int>& start) {
            return knap.Search3opt(start);
        });
        double val3opt = knap.evaluate(sol3opt);
        knap.printSolution(sol3opt, val3opt);
    } else {
        std::cerr << "Invalid task type. Use 1 for TSP, 2 for Knapsack.\n";
        return 1;
    }
    return 0;
}