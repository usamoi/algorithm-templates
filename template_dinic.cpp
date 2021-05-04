#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct Dinic {
    typedef long long ll;
    static const ll INF = 1LL << 32;
    struct Vertex {
        int cur, level;
        std::vector<int> outs;
    };
    struct Edge {
        int u, v;
        ll cap, flow;
    };
    int n;
    std::vector<Vertex> vertexs;
    std::vector<Edge> edges;
    void initial(int nn) {
        n = nn;
        vertexs.clear(), vertexs.resize(1 + n);
        edges.clear();
    }
    void addEdge(int u, int v, ll c) {
        vertexs[u].outs.push_back(edges.size()), edges.push_back(Edge{u, v, c, 0});
        vertexs[v].outs.push_back(edges.size()), edges.push_back(Edge{v, u, 0, 0});
    }
    bool dinicBfs(int s, int target) {
        for (int i = 0; i <= n; i++)
            vertexs[i].cur = 0,
            vertexs[i].level = -1;
        std::queue<int> q;
        vertexs[s].level = 0, q.push(s);
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (auto out : vertexs[x].outs) {
                Edge &e = edges[out];
                if (vertexs[e.v].level == -1 && e.cap > e.flow)
                    vertexs[e.v].level = vertexs[e.u].level + 1, q.push(e.v);
            }
        }
        return vertexs[target].level != -1;
    }
    ll dinicDfs(int s, int target, ll limit) {
        ll ans = 0, rem;
        if (s == target)
            return limit;
        for (int &i = vertexs[s].cur; limit > 0 && i < (int)vertexs[s].outs.size(); i++) {
            Edge &e = edges[vertexs[s].outs[i]];
            if (vertexs[e.u].level + 1 != vertexs[e.v].level ||
                0 == (rem = dinicDfs(e.v, target, std::min(limit, e.cap - e.flow))))
                continue;
            e.flow += rem, edges[vertexs[s].outs[i] ^ 1].flow -= rem;
            limit -= rem, ans += rem;
        }
        return ans;
    }
    ll dinic(int s, int t) {
        ll ans = 0;
        while (dinicBfs(s, t))
            ans += dinicDfs(s, t, INF);
        return ans;
    }
} g;
