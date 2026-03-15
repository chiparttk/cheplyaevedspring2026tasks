#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>

struct Vertex
{
    double x;
    double y;
};

class Graph
{
private:
    std::vector<Vertex> vertexs;
    int N;

    double calcDist(const Vertex& a, const Vertex& b){
        return std::sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
    }
public:
    Graph() : N(0.) {}
    Graph(int n ) : N(n){
        vertexs.resize(N);
    }

    void readVertexs(std::istream& in) {
        for(int i = 0; i < N; ++i) {
            in >> vertexs[i].x >> vertexs[i].y;
            // std::cout << i << ") " << vertexs[i].x << " " << vertexs[i].y << "\n";
        }
    }

    int getSize() const {return N;}

    double getDist(int v1, int v2) const {
        return std::sqrt((vertexs[v1].x - vertexs[v2].x)*(vertexs[v1].x - vertexs[v2].x) + 
            (vertexs[v1].y - vertexs[v2].y)*(vertexs[v1].y - vertexs[v2].y)); ;
    }
    ~Graph(){};
};

class Solver2opt
{
private:
    Graph graph;
public:
    Solver2opt() : graph(0) {}
    Solver2opt(Graph g) : graph(g) {}

    double calcTotalPathDist(const std::vector<int>& p) const{
        double total = 0.0;
        int n = p.size();
        for(int i = 0; i < n; ++i) {
            total += graph.getDist(p[i], p[(i+1)%n]);
        }
        return total;
    }
    std::pair<std::vector<int>, double> solve(){
        int n = graph.getSize();
        std::vector<int> path(n);

        for (int i = 0; i < n; ++i) path[i] = i;
        std::mt19937 gen(777);
        std::shuffle(path.begin(), path.end(), gen);
        double dist = calcTotalPathDist(path);
        double f = true;

        while(f) {
            f = false;

            for (int i = 1; i < n - 1; ++i){
                for (int j = i + 1; j < n; ++j) {
                    int a = path[i - 1];
                    int b = path[i];
                    int c = path[j];
                    int d = path[(j+1)%n];

                    double d1 = graph.getDist(a, b) + graph.getDist(c, d); 
                    double d2 = graph.getDist(a, c) + graph.getDist(b, d); 

                    if (d2 < d1) {
                        std::reverse(path.begin() + i, path.begin() + j + 1);
                        dist -= (d1 - d2);
                        f = true;
                    }
                }
            }
        }

        return {path, dist};
    }
    ~Solver2opt() {}
};

class Solver3opt
{
private:
    Graph graph;
public:
    Solver3opt() : graph(0) {}
    Solver3opt(Graph g) : graph(g) {}

    double calcTotalPathDist(const std::vector<int>& p) const{
        double total = 0.0;
        int n = p.size();
        for(int i = 0; i < n; ++i) {
            total += graph.getDist(p[i], p[(i+1)%n]);
        }
        return total;
    }
    std::pair<std::vector<int>, double> solve(){
        int n = graph.getSize();
        std::vector<int> path(n);

        for (int i = 0; i < n; ++i) path[i] = i;
        std::mt19937 gen(777);
        std::shuffle(path.begin(), path.end(), gen);
        double dist = calcTotalPathDist(path);
        double f = true;

        while(f) {
            f = false;

            for (int i = 1; i < n - 1; ++i){
                for (int j = i + 1; j < n ; ++j) {
                    for (int k = j + 1; k < n +1; ++k) {
                        int a = path[i - 1];
                        int b = path[i];
                        int c = path[j - 1];
                        int d = path[j];
                        int e = path[k - 1];
                        int f = path[k % n];

                        double d0 = graph.getDist(a, b) + graph.getDist(c, d) + graph.getDist(e, f); 
                        double d1 = graph.getDist(a, c) + graph.getDist(b, d) + graph.getDist(e, f); 
                        double d2 = graph.getDist(a, b) + graph.getDist(c, e) + graph.getDist(d, f); 
                        double d3 = graph.getDist(a, e) + graph.getDist(d, c) + graph.getDist(b, f); 
                        double d4 = graph.getDist(a, d) + graph.getDist(e, b) + graph.getDist(c, f); 
                        double d5 = graph.getDist(a, c) + graph.getDist(b, e) + graph.getDist(d, f); 
                        double d6 = graph.getDist(a, e) + graph.getDist(d, b) + graph.getDist(c, f); 
                        double d7 = graph.getDist(a, d) + graph.getDist(e, c) + graph.getDist(b, f); 
                        

                        if (d1 < d0) {
                            std::reverse(path.begin() + i, path.begin() + j);
                            dist -= (d0 - d1);
                            f = true;
                        }
                        else if (d2 < d0) {
                            std::reverse(path.begin() + j, path.begin() + k);
                            dist -= (d0 - d2);
                            f = true;
                        }
                        else if (d3 < d0) {
                            std::reverse(path.begin() + i, path.begin() + k);
                            dist -= (d0 - d3);
                            f = true;
                        }
                        else if (d4 < d0) {
                            std::rotate(path.begin() + i, path.begin() + j, path.begin() + k);
                            dist -= (d0 - d4);
                            f = true;
                        }
                        else if (d5 < d0) {
                            std::reverse(path.begin() + i, path.begin() + j);
                            std::reverse(path.begin() + j, path.begin() + k);
                            dist -= (d0 - d5);
                            f = true;
                        }
                        else if (d6 < d0) {
                            std::reverse(path.begin() + i, path.begin() + k);
                            std::rotate(path.begin() + i, path.begin() + j, path.begin() + k);
                            dist -= (d0 - d6);
                            f = true;
                        }
                        else if (d7 < d0) {
                            std::reverse(path.begin() + i, path.begin() + j);
                            std::rotate(path.begin() + i, path.begin() + j, path.begin() + k);
                            dist -= (d0 - d7);
                            f = true;
                        }
                    }
                }
            }
        }

        return {path, dist};
    }
    ~Solver3opt() {}
};

class Test
{
public:
    static void run(const std::string& filename){
        std::ifstream file(filename);

        if(!file.is_open()){
            std::cout << "ERROR: Cannot open file " << filename << "\n";
            return;
        }

        int n;
        if(!(file >> n)) {
            std::cout << "ERROR: Cannot read N from file \n";
            return;
        }
        Graph graph(n);
        graph.readVertexs(file);
        file.close();
        Solver2opt solver2(graph);
        Solver3opt solver3(graph);

        std::pair<std::vector<int>, double> result2 = solver2.solve();
        std::cout << "best distance (2opt): " << result2.second << "\n";
        
        std::pair<std::vector<int>, double> result3 = solver3.solve();
        std::cout << "best distance (3opt): " << result3.second << "\n";
        // std::cout <<"path: " << "\n";
        // for (int i = 0; i <= n; ++i){
        //     std::cout << result.first[i%n] << "-";
        // }
        // std::cout << "|\n";
    }
};

int main() {

    std::string filename = "../tsp_data/tsp_51_1";
    Test::run(filename);
    return 0;
}