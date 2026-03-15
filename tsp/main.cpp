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

class Solver
{
private:
    Graph graph;
public:
    Solver() : graph(0) {}
    Solver(Graph g) : graph(g) {}

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

            for (int i = 1; i < n; ++i){
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
    ~Solver() {}
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
        Solver solver(graph);

        std::pair<std::vector<int>, double> result = solver.solve();

        std::cout << "best distance: " << result.second << "\n";
        // std::cout <<"path: " << "\n";
        // for (int i = 0; i <= n; ++i){
        //     std::cout << result.first[i%n] << "-";
        // }
        // std::cout << "|\n";
    }
};

int main() {

    std::string filename = "../tsp_data/tsp_1817_1";
    Test::run(filename);
    return 0;
}