#ifndef TASK_H
#define TASK_H

#include <vector>

class Task {
public:
    virtual ~Task() = default;

    virtual void load(std::istream& in) = 0;
    virtual double evaluate(const std::vector<int>& solution) const = 0;
    virtual bool isFeasible(const std::vector<int>& solution) const = 0;
    virtual std::vector<int> randomSolution() = 0;
    virtual void printSolution(const std::vector<int>& solution, double value) const = 0;
};

#endif