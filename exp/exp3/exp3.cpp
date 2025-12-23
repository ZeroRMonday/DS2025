#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <cstring>
#include <unordered_set>

using namespace std;

// 图的数据结构，邻接矩阵表示
class Graph {
public:
    vector<vector<int>> adjMatrix;
    int V; // 顶点数

    Graph(int vertices) {
        V = vertices;
        adjMatrix.resize(V, vector<int>(V, 0)); // 初始化邻接矩阵
    }

    // 添加边
    void addEdge(int u, int v, int weight = 1) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight;  // 如果是无向图，设置反向边
    }

    // 输出邻接矩阵
    void printAdjMatrix() {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    // 广度优先搜索 BFS
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front();
            cout << node << " ";
            q.pop();

            for (int i = 0; i < V; i++) {
                if (adjMatrix[node][i] != 0 && !visited[i]) { // 有边且未访问
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
        cout << endl;
    }

    // 深度优先搜索 DFS
    void DFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;

        s.push(start);

        while (!s.empty()) {
            int node = s.top();
            s.pop();

            if (!visited[node]) {
                visited[node] = true;
                cout << node << " ";
            }

            for (int i = 0; i < V; i++) {
                if (adjMatrix[node][i] != 0 && !visited[i]) {
                    s.push(i);
                }
            }
        }
        cout << endl;
    }

    // Dijkstra 算法计算最短路径
    void dijkstra(int start) {
        vector<int> dist(V, INT_MAX);
        vector<bool> visited(V, false);
        dist[start] = 0;

        for (int i = 0; i < V - 1; i++) {
            int u = -1;
            for (int j = 0; j < V; j++) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                    u = j;
                }
            }

            visited[u] = true;

            for (int v = 0; v < V; v++) {
                if (adjMatrix[u][v] != 0 && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                }
            }
        }

        // 输出最短路径
        for (int i = 0; i < V; i++) {
            cout << "Distance from " << start << " to " << i << ": " << dist[i] << endl;
        }
    }

    // 最小生成树（使用 Prim 算法）
    void prim() {
        vector<int> parent(V, -1);
        vector<int> key(V, INT_MAX);
        vector<bool> inMST(V, false);

        key[0] = 0;
        for (int count = 0; count < V - 1; count++) {
            int u = -1;
            for (int v = 0; v < V; v++) {
                if (!inMST[v] && (u == -1 || key[v] < key[u])) {
                    u = v;
                }
            }

            inMST[u] = true;

            for (int v = 0; v < V; v++) {
                if (adjMatrix[u][v] != 0 && !inMST[v] && adjMatrix[u][v] < key[v]) {
                    key[v] = adjMatrix[u][v];
                    parent[v] = u;
                }
            }
        }

        // 输出最小生成树
        cout << "Edge \t Weight" << endl;
        for (int i = 1; i < V; i++) {
            cout << parent[i] << " - " << i << " \t " << adjMatrix[i][parent[i]] << endl;
        }
    }

    // 计算双连通分量
    void tarjanBCC(int u, int disc[], int low[], stack<int>& st, vector<bool>& inStack, vector<unordered_set<int>>& bcc, int& time) {
        disc[u] = low[u] = ++time;
        st.push(u);
        inStack[u] = true;

        for (int v = 0; v < V; v++) {
            if (adjMatrix[u][v] != 0) {
                if (disc[v] == -1) {
                    tarjanBCC(v, disc, low, st, inStack, bcc, time);
                    low[u] = min(low[u], low[v]);
                }
                else if (inStack[v]) {
                    low[u] = min(low[u], disc[v]);
                }
            }
        }

        if (low[u] == disc[u]) {
            unordered_set<int> component;
            while (st.top() != u) {
                int v = st.top();
                component.insert(v);
                inStack[v] = false;
                st.pop();
            }
            component.insert(u);
            inStack[u] = false;
            st.pop();
            bcc.push_back(component);
        }
    }

    // 调用函数来计算并输出双连通分量
    void findBCC() {
        int disc[V], low[V];
        vector<bool> inStack(V, false);
        stack<int> st;
        vector<unordered_set<int>> bcc;
        int time = -1;

        fill(disc, disc + V, -1);

        for (int i = 0; i < V; i++) {
            if (disc[i] == -1) {
                tarjanBCC(i, disc, low, st, inStack, bcc, time);
            }
        }

        cout << "Biconnected components:" << endl;
        for (auto& component : bcc) {
            for (int node : component) {
                cout << node << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    // 创建图1
    Graph g1(6);

    g1.addEdge(0, 1, 1);
    g1.addEdge(0, 2, 1);
    g1.addEdge(1, 3, 1);
    g1.addEdge(1, 4, 1);
    g1.addEdge(2, 5, 1);
    g1.addEdge(3, 5, 1);
    g1.addEdge(4, 5, 1);

    cout << "Adjacency Matrix of Graph 1:" << endl;
    g1.printAdjMatrix();

    cout << "BFS starting from node A:" << endl;
    g1.BFS(0); // A点为0

    cout << "DFS starting from node A:" << endl;
    g1.DFS(0); // A点为0

    cout << "Dijkstra's Shortest Path from node A:" << endl;
    g1.dijkstra(0); // A点为0

    cout << "Prim's Minimum Spanning Tree:" << endl;
    g1.prim();

    cout << "Tarjan's Biconnected Components from Graph 1:" << endl;
    g1.findBCC();

    // 创建图2
    Graph g2(7);
    g2.addEdge(0, 1, 1);
    g2.addEdge(1, 2, 1);
    g2.addEdge(2, 3, 1);
    g2.addEdge(3, 4, 1);
    g2.addEdge(4, 5, 1);
    g2.addEdge(5, 6, 1);
    g2.addEdge(3, 6, 1);

    cout << "Tarjan's Biconnected Components from Graph 2:" << endl;
    g2.findBCC();

    return 0;
}

