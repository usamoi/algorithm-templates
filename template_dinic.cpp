#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct Dinic {
    typedef long long ll;
    static const ll INF = 1LL << 32;
    struct Edge {
        int u, v;
        ll cap, flow = 0;
    };
    struct Vertex {
        int cur, level;
        std::vector<int> outs;
    };
    std::vector<Vertex> V;
    std::vector<Edge> E;
    void initial(int n) {
        V.clear(), V.resize(1 + n);
        E.clear();
    }
    void addEdge(int u, int v, ll c) {
        V[u].outs.push_back(E.size()), E.push_back(Edge{u, v, c, 0});
        V[v].outs.push_back(E.size()), E.push_back(Edge{v, u, 0, 0});
    }
    bool dinicBfs(int s, int target) {
        for (auto &u : V)
            u.cur = 0, u.level = -1;
        std::queue<int> q;
        V[s].level = 0, q.push(s);
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (auto out : V[x].outs) {
                Edge &e = E[out];
                if (V[e.v].level == -1 && e.cap > e.flow)
                    V[e.v].level = V[e.u].level + 1, q.push(e.v);
            }
        }
        return V[target].level != -1;
    }
    ll dinicDfs(int s, int target, ll limit) {
        ll ans = 0, rem;
        if (s == target)
            return limit;
        for (int &i = V[s].cur; limit > 0 && i < (int)V[s].outs.size(); i++) {
            Edge &e = E[V[s].outs[i]];
            if (V[e.u].level + 1 != V[e.v].level ||
                0 == (rem = dinicDfs(e.v, target, std::min(limit, e.cap - e.flow))))
                continue;
            e.flow += rem, E[V[s].outs[i] ^ 1].flow -= rem;
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
};

int main() {
    typedef Dinic::ll ll;
    Dinic g;
    int s, t;
    Dinic::ll n, m;
    std::cin >> n >> m >> s >> t;
    g.initial(n);
    for (ll i = 0; i < m; i++) {
        ll u, v, c;
        std::cin >> u >> v >> c;
        g.addEdge(u, v, c);
    }
    std::cout << g.dinic(s, t) << std::endl;
    return 0;
}