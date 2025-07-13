#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Weighted random neighbor selection
int weightedRandomNeighbor(const vector<pair<int, double>>& neighbors) {
    if (neighbors.empty()) return -1;

    double totalWeight = 0;
    for (const auto& p : neighbors) {
        totalWeight += p.second;
    }

    double r = ((double)rand() / RAND_MAX) * totalWeight;
    double cumulative = 0;

    for (const auto& p : neighbors) {
        cumulative += p.second;
        if (r <= cumulative) return p.first;
    }

    return neighbors.back().first;  // fallback
}

// Read weighted undirected graph from file
void readGraph(const string& filename,
               unordered_map<int, vector<pair<int, double>>>& graph,
               unordered_set<int>& nodeSet) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int u, v;
        double weight;
        if (!(ss >> u >> v >> weight)) {
            cerr << "Invalid line in file: " << line << endl;
            continue;
        }

        graph[u].push_back(make_pair(v, weight));
        graph[v].push_back(make_pair(u, weight));  // undirected

        nodeSet.insert(u);
        nodeSet.insert(v);
    }

    file.close();
}

// Pick a random node from a set
int getRandomNode(const unordered_set<int>& nodeSet) {
    if (nodeSet.empty()) return -1;
    vector<int> nodes(nodeSet.begin(), nodeSet.end());
    return nodes[rand() % nodes.size()];
}

int main() {
    srand(time(0));

    unordered_map<int, vector<pair<int, double>>> graphA, graphB;
    unordered_set<int> nodesA, nodesB;

    readGraph("nov_2.txt", graphA, nodesA);
    readGraph("nov_3.txt", graphB, nodesB);

    int pathLength, numWalks;
    cout << "Enter path length: ";
    cin >> pathLength;
    cout << "Enter number of walks: ";
    cin >> numWalks;

    cout << "\n--- Starting Weighted Random Walks ---\n";

    for (int walk = 1; walk <= numWalks; ++walk) {
        bool inA = rand() % 2;
        int current = inA ? getRandomNode(nodesA) : getRandomNode(nodesB);

        if (current == -1) {
            cout << "Walk " << walk << " skipped: empty starting network.\n";
            continue;
        }

        cout << "\nWalk " << walk << " starts at " << (inA ? "A-" : "B-") << current << endl;

        for (int step = 1; step <= pathLength; ++step) {
            cout << "  Step " << step << ": " << (inA ? "A-" : "B-") << current << endl;

            unordered_map<int, vector<pair<int, double>>>& graph = inA ? graphA : graphB;

            if (graph.find(current) == graph.end() || graph[current].empty()) {
                cout << "    No neighbors. Ending walk early.\n";
                break;
            }

            int next = weightedRandomNeighbor(graph[current]);
            if (next == -1) {
                cout << "    Failed to pick a neighbor. Ending walk.\n";
                break;
            }

            bool existsInOther = inA ? (nodesB.count(next) > 0) : (nodesA.count(next) > 0);
            current = next;

            if (existsInOther) {
                inA = !inA;
                cout << "    Jumped to " << (inA ? "A" : "B") << " via shared node " << current << endl;
            }
        }
    }

    return 0;
}
