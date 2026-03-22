#include "tsp.h"
#include <iostream>
#include <random>

void TSP::load(std::istream& in) {
    in >> vertexs;
    x.resize(vertexs);
    y.resize(vertexs);
    for (int i = 0; i < vertexs; ++i) {
        in >> x[i] >> y[i];
    }
}

double TSP::evaluate(const std::vector<int>& solution) const {
    double len = 0.0;
    for (int i = 0; i < vertexs; ++i) {
        int cur = solution[i];
        int nxt = solution[(i+1) % vertexs];
        len += distance(cur, nxt);
    }
    return len;
}

bool TSP::isFeasible(const std::vector<int>& solution) const {
    std::vector<bool> visited(vertexs, false);
    for (int city : solution) {
        if (city < 0 || city >= vertexs) return false;
        if (visited[city]) return false;
        visited[city] = true;
    }
    return true;
}

std::vector<int> TSP::randomSolution() {
    std::vector<int> path(vertexs);
    for (int i = 0; i < vertexs; ++i) path[i] = i;
    static std::mt19937 gen(777);
    std::shuffle(path.begin(), path.end(), gen);
    return path;
}

void TSP::printSolution(const std::vector<int>& solution, double value) const {
    std::cout << value << "\n";
    for (int i = 0; i < vertexs; ++i) {
        std::cout << solution[i] << " ";
    }
    std::cout << "\n";
}

std::vector<int> TSP::Search2opt(const std::vector<int>& start) const {
    std::vector<int> best = start;
    double bestDist = evaluate(best);
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 1; i < vertexs - 1; ++i) {
            for (int j = i + 1; j < vertexs; ++j) {
                int a = best[i-1];
                int b = best[i];
                int c = best[j];
                int d = best[(j+1) % vertexs];

                double d1 = distance(a,b) + distance(c,d);
                double d2 = distance(a,c) + distance(b,d);

                if (d2 < d1) {
                    std::reverse(best.begin() + i, best.begin() + j + 1);
                    bestDist -= (d1 - d2);
                    improved = true;
                    break;
                }
            }
            if (improved) break;
        }
    }
    return best;
}

std::vector<int> TSP::Search3opt(const std::vector<int>& start) const {
    std::vector<int> best = start;
    double bestDist = evaluate(best);
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 1; i < vertexs - 1; ++i) {
            for (int j = i + 1; j < vertexs; ++j) {
                for (int k = j + 1; k < vertexs + 1; ++k) {
                    int a = best[i-1];
                    int b = best[i];
                    int c = best[j-1];
                    int d = best[j];
                    int e = best[k-1];
                    int f = best[k % vertexs];

                    double d0 = distance(a,b) + distance(c,d) + distance(e,f);
                    double d1 = distance(a,c) + distance(b,d) + distance(e,f);
                    double d2 = distance(a,b) + distance(c,e) + distance(d,f);
                    double d3 = distance(a,e) + distance(d,c) + distance(b,f);
                    double d4 = distance(a,d) + distance(e,b) + distance(c,f);
                    double d5 = distance(a,c) + distance(b,e) + distance(d,f);
                    double d6 = distance(a,e) + distance(d,b) + distance(c,f);
                    double d7 = distance(a,d) + distance(e,c) + distance(b,f);

                    if (d1 < d0 ) {
                        std::reverse(best.begin() + i, best.begin() + j);
                        bestDist -= (d0 - d1);
                        improved = true;
                        break;
                    }
                    if (d2 < d0) {
                        std::reverse(best.begin() + j, best.begin() + k);
                        bestDist -= (d0 - d2);
                        improved = true;
                        break;
                    }
                    if (d3 < d0) {
                        std::reverse(best.begin() + i, best.begin() + k);
                        bestDist -= (d0 - d3);
                        improved = true;
                        break;
                    }
                    if (d4 < d0) {
                        std::rotate(best.begin() + i, best.begin() + j, best.begin() + k);
                        bestDist -= (d0 - d4);
                        improved = true;
                        break;
                    }
                    if (d5 < d0) {
                        std::reverse(best.begin() + i, best.begin() + j);
                        std::reverse(best.begin() + j, best.begin() + k);
                        bestDist -= (d0 - d5);
                        improved = true;
                        break;
                    }
                    if (d6 < d0) {
                        std::reverse(best.begin() + i, best.begin() + k);
                        std::rotate(best.begin() + i, best.begin() + j, best.begin() + k);
                        bestDist -= (d0 - d6);
                        improved = true;
                        break;
                    }
                    if (d7 < d0) {
                        std::reverse(best.begin() + i, best.begin() + j);
                        std::rotate(best.begin() + i, best.begin() + j, best.begin() + k);
                        bestDist -= (d0 - d7);
                        improved = true;
                        break;
                    }
                }
                if (improved) break;
            }
            if (improved) break;
        }
    }
    return best;
}