#include <bits/stdc++.h>


using namespace std;

// Graph: each node maps to a list of (neighbor, weight)
unordered_map<int, vector<pair<int, float>>> graph;

int pathLength;   // how many steps per random walk
int totalWalks;   // how many random walks to perform

// Function to read graph from a file
void readGraph(string filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        int from, to;
        float weight;

        ss >> from >> to >> weight;

        graph[from].push_back(make_pair(to, weight));
    }

    file.close();
}

// Function to pick a neighbor randomly based on weight
int chooseNextNode(vector<pair<int, float>> neighbors) {
    float totalWeight = 0;

    // Step 1: Calculate total weight
    for (int i = 0; i < neighbors.size(); i++) {
        totalWeight += neighbors[i].second;
    }

    // Step 2: Generate random float between 0 and totalWeight
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(0.0, totalWeight);
    float randValue = dist(gen);

    // Step 3: Pick the neighbor where the random value falls
    float sum = 0;
    for (int i = 0; i < neighbors.size(); i++) {
        sum += neighbors[i].second;
        if (randValue <= sum) {
            return neighbors[i].first;
        }
    }

    // fallback: return last neighbor (shouldn't happen)
    return neighbors.back().first;
}

// Function to perform one random walk starting from a node
vector<int> doRandomWalk(int startNode) {
    vector<int> path;
    path.push_back(startNode);
    int currentNode = startNode;

    for (int step = 0; step < pathLength - 1; step++) {
        if (graph.find(currentNode) == graph.end() || graph[currentNode].empty()) {
            break; // no outgoing edges
        }

        int nextNode = chooseNextNode(graph[currentNode]);
        path.push_back(nextNode);
        currentNode = nextNode;
    }

    return path;
}

int main() {
    srand(time(0)); // seed random numbers

    // Step 1: Read the graph
    readGraph("nov_2.txt");

    // Step 2: Ask user input
    cout << "Enter path length: ";
    cin >> pathLength;
    cout << "Enter number of walks: ";
    cin >> totalWalks;

    // Step 3: Prepare list of all starting nodes
    vector<int> startNodes;
    for (auto it = graph.begin(); it != graph.end(); it++) {
        startNodes.push_back(it->first);
    }

    unordered_set<int> visitedNodes; // track all visited nodes

    // Step 4: Perform random walks
    for (int walk = 1; walk <= totalWalks; ) {
        int randomIndex = rand() % startNodes.size();
        int start = startNodes[randomIndex];

        vector<int> walkPath = doRandomWalk(start);

        if (walkPath.size() < 2) continue; // skip very short walks

        cout << "Walk " << walk << ": [";
        for (int i = 0; i < walkPath.size(); i++) {
            cout << walkPath[i];
            if (i != walkPath.size() - 1) cout << ", ";
            visitedNodes.insert(walkPath[i]);
        }
        cout << "]" << endl;

        walk++;
    }

    // Step 5: Check which nodes were never visited
    unordered_set<int> allNodes;
    for (auto it = graph.begin(); it != graph.end(); it++) {
        allNodes.insert(it->first);
        for (int j = 0; j < it->second.size(); j++) {
            allNodes.insert(it->second[j].first);
        }
    }

    vector<int> notVisited;
    for (int node : allNodes) {
        if (visitedNodes.find(node) == visitedNodes.end()) {
            notVisited.push_back(node);
        }
    }

    // Step 6: Show results
    if (notVisited.empty()) {
        cout << "\nAll nodes were visited at least once.\n";
    } else {
        cout << "\nNodes not visited: [";
        for (int i = 0; i < notVisited.size(); i++) {
            cout << notVisited[i];
            if (i != notVisited.size() - 1) cout << ", ";
        }
        cout << "]\n";
    }

    return 0;
}
