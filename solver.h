#ifndef SOLVER_H
#define SOLVER_H

#include "task.h"
#include <functional>
#include <vector>

class Solver {
public:
    explicit Solver() {}

    template <typename F>
    std::vector<int> solve(Task* problem, F improvementFunction) const {
        std::vector<int> start = problem->randomSolution();
        return improvementFunction(start);
    }
};

#endif