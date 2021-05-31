#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>

struct Bellman {
    typedef long long ll;
    static const ll INF = 1LL << 32;
    struct Edge {
        int u, v;
        ll cap, cost, flow;
    };
    struct Vertex {
        std::vector<int> e;
        ll d;
        int backEdge;
        int inq;
    };
    std::vector<Vertex> V;
    std::vector<Edge> E;
    void initial(int n) {
        V.clear(), V.resize(1 + n);
        E.clear();
    }
    void addEdge(int u, int v, ll cap, ll cost) {
        V[u].e.push_back(E.size()), E.push_back(Edge{u, v, cap, cost, 0});
        V[v].e.push_back(E.size()), E.push_back(Edge{v, u, 0, -cost, 0});
    }
    bool bfs(int s, int t) {
        for (auto &u : V) {
            u.d = INF, u.inq = 0;
        }
        V[s].d = 0, V[s].backEdge = -1;
        std::queue<int> q;
        V[s].inq = 1, q.push(s);
        while (!q.empty()) {
            int f = q.front();
            V[f].inq = 0, q.pop();
            for (int i = 0; i < (int)V[f].e.size(); i++) {
                Edge &e = E[V[f].e[i]];
                if (e.cap == e.flow)
                    continue;
                if (V[e.v].d > e.cost + V[e.u].d) {
                    V[e.v].d = e.cost + V[e.u].d;
                    V[e.v].backEdge = V[f].e[i];
                    if (V[e.v].inq == 0) {
                        V[e.v].inq = 1, q.push(e.v);
                    }
                }
            }
        }
        return V[t].d != INF;
    }
    void bellman(int s, int t, ll &flow, ll &cost) {
        flow = 0, cost = 0;
        while (bfs(s, t)) {
            ll limit = INF;
            for (int loc = V[t].backEdge; loc != -1; loc = V[E[loc].u].backEdge)
                limit = std::min(limit, E[loc].cap - E[loc].flow);
            for (int loc = V[t].backEdge; loc != -1; loc = V[E[loc].u].backEdge)
                E[loc].flow += limit, E[1 ^ loc].flow -= limit, cost += limit * E[loc].cost;
            flow += limit;
        }
    }
};
