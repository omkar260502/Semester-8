#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Graph class representing an undirected graph using adjacency list
class Graph
{
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    // Function to add an edge to the graph
    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Sequential Breadth First Search algorithm
    vector<int> sequentialBFS(int start)
    {
        vector<bool> visited(V, false);
        vector<int> result;
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            result.push_back(u);

            for (int v : adj[u])
            {
                if (!visited[v])
                {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        return result;
    }

    // Sequential Depth First Search algorithm
    vector<int> sequentialDFS(int start)
    {
        vector<bool> visited(V, false);
        vector<int> result;
        stack<int> s;
        s.push(start);
        visited[start] = true;

        while (!s.empty())
        {
            int u = s.top();
            s.pop();
            result.push_back(u);

            for (int v : adj[u])
            {
                if (!visited[v])
                {
                    visited[v] = true;
                    s.push(v);
                }
            }
        }

        return result;
    }

    // Parallel Breadth First Search algorithm
    vector<int> parallelBFS(int start)
    {
        vector<bool> visited(V, false);
        vector<int> result;
        queue<int> q;
        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            result.push_back(u);

#pragma omp parallel for
            for (int i = 0; i < adj[u].size(); ++i)
            {
                int v = adj[u][i];
                if (!visited[v])
                {
#pragma omp critical
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }

        return result;
    }

    // Parallel Depth First Search algorithm
    vector<int> parallelDFS(int start)
    {
        vector<bool> visited(V, false);
        vector<int> result;
        stack<int> s;
        s.push(start);
        visited[start] = true;

        while (!s.empty())
        {
            int u = s.top();
            s.pop();
            result.push_back(u);

#pragma omp parallel for
            for (int i = 0; i < adj[u].size(); ++i)
            {
                int v = adj[u][i];
                if (!visited[v])
                {
#pragma omp critical
                    {
                        visited[v] = true;
                        s.push(v);
                    }
                }
            }
        }

        return result;
    }
};

int main()
{
    Graph g(7); // Create a graph with 7 vertices
    // Add edges
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    // Perform sequential BFS from vertex 0
    cout << "Sequential BFS traversal: ";
    auto start_time = high_resolution_clock::now();
    vector<int> sequential_bfs_result = g.sequentialBFS(0);
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time).count();
    cout << "Sequential BFS took " << duration << " microseconds" << endl;

    for (int i = 0; i < sequential_bfs_result.size(); ++i)
    {
        cout << sequential_bfs_result[i] << " ";
    }
    cout << endl;

    // Perform sequential DFS from vertex 0
    cout << "Sequential DFS traversal: ";
    start_time = high_resolution_clock::now();
    vector<int> sequential_dfs_result = g.sequentialDFS(0);
    end_time = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end_time - start_time).count();
    cout << "Sequential DFS took " << duration << " microseconds" << endl;

    for (int i = 0; i < sequential_dfs_result.size(); ++i)
    {
        cout << sequential_dfs_result[i] << " ";
    }
    cout << endl;

    // Perform parallel BFS from vertex 0
    cout << "Parallel BFS traversal: ";
    start_time = high_resolution_clock::now();
    vector<int> parallel_bfs_result = g.parallelBFS(0);
    end_time = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end_time - start_time).count();
    cout << "Parallel BFS took " << duration << " microseconds" << endl;

    for (int i = 0; i < parallel_bfs_result.size(); ++i)
    {
        cout << parallel_bfs_result[i] << " ";
    }
    cout << endl;

    // Perform parallel DFS from vertex 0
    cout << "Parallel DFS traversal: ";
    start_time = high_resolution_clock::now();
    vector<int> parallel_dfs_result = g.parallelDFS(0);
    end_time = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end_time - start_time).count();
    cout << "Parallel DFS took " << duration << " microseconds" << endl;

    for (int i = 0; i < parallel_dfs_result.size(); ++i)
    {
        cout << parallel_dfs_result[i] << " ";
    }
    cout << endl;

    return 0;
}