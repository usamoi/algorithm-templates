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
        V[u].outs.push_back(E.size()), E.push_back(Edge{u, v, c});
        V[v].outs.push_back(E.size()), E.push_back(Edge{v, u, 0});
    }
    bool dinicBfs(int S, int T) {
        for (auto &u : V)
            u.cur = 0, u.level = -1;
        std::queue<int> q;
        V[S].level = 0, q.push(S);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto out : V[u].outs) {
                Edge &e = E[out];
                if (V[e.v].level == -1 && e.cap > e.flow)
                    V[e.v].level = V[e.u].level + 1, q.push(e.v);
            }
        }
        return V[T].level != -1;
    }
    ll dinicDfs(int u, int T, ll limit) {
        ll ans = 0, rem;
        if (u == T)
            return limit;
        for (int &i = V[u].cur; limit > 0 && i < (int)V[u].outs.size(); i++) {
            Edge &e = E[V[u].outs[i]];
            if (V[e.u].level + 1 != V[e.v].level ||
                0 == (rem = dinicDfs(e.v, T, std::min(limit, e.cap - e.flow))))
                continue;
            e.flow += rem, E[V[u].outs[i] ^ 1].flow -= rem;
            limit -= rem, ans += rem;
        }
        return ans;
    }
    ll dinic(int S, int T) {
        ll ans = 0;
        while (dinicBfs(S, T))
            ans += dinicDfs(S, T, INF);
        return ans;
    }
};

struct Lowerbound {
    typedef Dinic::ll ll;
    struct Edge {
        int u, v;
        ll lower, upper;
        int cor = -1;
    };
    std::vector<Edge> E;
    int n;
    void initial(int _n) {
        n = _n;
        E.clear();
    }
    void addEdge(int u, int v, ll lower, ll upper) {
        E.push_back(Edge{u, v, lower, upper});
    }
    Dinic g;
    int vS, vT;
    void build() {
        g.initial(n + 2), vS = n + 1, vT = n + 2;
        auto lowersum = std::vector<ll>(1 + n);
        for (auto &e : E) {
            lowersum[e.v] += +e.lower;
            lowersum[e.u] += -e.lower;
            e.cor = g.E.size(), g.addEdge(e.u, e.v, e.upper - e.lower);
        }
        for (int i = 1; i <= n; i++) {
            if (lowersum[i] > 0) {
                g.addEdge(vS, i, +lowersum[i]);
            }
            if (lowersum[i] < 0) {
                g.addEdge(i, vT, -lowersum[i]);
            }
        }
    }
    bool check() {
        for (auto ex : g.V[vS].outs) {
            auto &e = g.E[ex];
            if (e.flow < e.cap)
                return false;
        }
        return true;
    }
    bool possible() {
        build();
        g.dinic(vS, vT);
        return check();
    }
    bool maxflow(int s, int t) {
        build();
        g.addEdge(t, s, Dinic::INF);
        g.dinic(vS, vT);
        if (!check())
            return false;
        g.dinic(s, t);
        return true;
    }
    bool minflow(int s, int t) {
        build();
        int ie = g.E.size();
        g.addEdge(t, s, Dinic::INF);
        g.dinic(vS, vT);
        if (!check())
            return false;
        g.E[ie].cap = g.E[ie ^ 1].cap = 0;
        g.dinic(t, s);
        return true;
    }
};

int main() {
    typedef long long ll;
    int n, m, s, t;
    scanf("%d%d%d%d", &n, &m, &s, &t);
    Lowerbound bd;
    bd.initial(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        ll lower, upper;
        scanf("%d%d%lld%lld", &u, &v, &lower, &upper);
        bd.addEdge(u, v, lower, upper);
    }
    if (bd.minflow(s, t)) {
        ll ans = 0;
        for (const auto &e : bd.E)
            if (e.u == s)
                ans += e.lower + bd.g.E[e.cor].flow;
        printf("%lld\n", ans);
    } else {
        printf("please go home to sleep\n");
    }
    return 0;
}