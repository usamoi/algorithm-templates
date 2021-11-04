#include <algorithm>
#include <cstdio>
#include <queue>
#include <vector>

struct Dijkstra {
    typedef long long ll;
    static const ll INF = 1e18;
    static const ll LARGE = 1e12;
    struct Edge {
        int u, v;
        ll cap, cost, flow, raw_cap;
    };
    struct Vertex {
        std::vector<int> e;
        ll d, h = 0;
        int prev;
        int inq;
    };
    std::vector<Vertex> V;
    std::vector<Edge> E;
    void initial(int n) {
        V.clear(), V.resize(1 + n);
        E.clear();
    }
    void addEdge(int u, int v, ll cap, ll cost) {
        V[u].e.push_back(E.size()), E.push_back({u, v, cap, cost, 0, cap});
        V[v].e.push_back(E.size()), E.push_back({v, u, 0, -cost, 0, 0});
    }
    ll cost(const Edge &e) const {
        return e.cost + V[e.u].h - V[e.v].h;
    }
    bool bellman(int s, int t) {
        for (auto &u : V) {
            u.d = INF, u.inq = false;
        }
        std::queue<int> q;
        V[s].prev = -1, V[s].d = 0;
        V[s].inq = true, q.push(s);
        while (!q.empty()) {
            int u = q.front();
            V[u].inq = false, q.pop();
            for (auto idx : V[u].e) {
                Edge &e = E[idx];
                if (e.cap == e.flow)
                    continue;
                if (V[e.v].d > V[u].d + e.cost) {
                    V[e.v].prev = idx, V[e.v].d = V[u].d + e.cost;
                    if (!V[e.v].inq) {
                        V[e.v].inq = true, q.push(e.v);
                    }
                }
            }
        }
        return V[t].d != INF;
    }
    bool dijkstra(int s, int t) {
        std::priority_queue<std::pair<ll, int>, std::vector<std::pair<ll, int>>, std::greater<std::pair<ll, int>>> q;
        for (auto &u : V) {
            u.d = INF;
        }
        V[s].prev = -1, q.push(std::make_pair(V[s].d = 0, s));
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (V[u].d != d)
                continue;
            for (auto idx : V[u].e) {
                const Edge &e = E[idx];
                if (e.flow == e.cap)
                    continue;
                if (V[e.v].d > V[e.u].d + cost(e)) {
                    V[e.v].prev = idx, q.push(std::make_pair(V[e.v].d = V[e.u].d + cost(e), e.v));
                }
            }
        }
        return V[t].d != INF;
    }
    void successive_shortest_path(int s, int t, ll &out_flow, ll &out_cost) {
        int n = V.size() - 1;
        int m = E.size() / 2;
        out_flow = 0;
        for (bellman(s, t); dijkstra(s, t);) {
            for (int u = 0; u <= n; u++) {
                V[u].h += V[u].d;
            }
            ll flow = INF;
            for (int x = V[t].prev; x != -1; x = V[E[x].u].prev)
                flow = std::min(flow, E[x].cap - E[x].flow);
            for (int x = V[t].prev; x != -1; x = V[E[x].u].prev)
                E[x].flow += flow, E[x ^ 1].flow -= flow;
            out_flow += flow;
        }
        out_cost = 0;
        for (int i = 0; i < m; i++)
            out_cost += E[i << 1].flow * E[i << 1].cost;
    }
    void capacity_scaling(int s, int t, ll &out_flow, ll &out_cost) {
        int n = V.size() - 1;
        int m = E.size() / 2;
        addEdge(t, s, LARGE, -LARGE);
        for (auto &e : E) {
            e.cap = 0;
        }
        V.push_back(Vertex());
        for (int i = 0; i <= n; i++) {
            V[n + 1].e.push_back(E.size()), E.push_back({n + 1, i, 1, 0, 0, 0});
        }
        for (int i = 40; i >= 0; i--) {
            for (int j = 0; j < m + 1; j++) {
                E[j << 1 | 0].cap <<= 1, E[j << 1 | 0].flow <<= 1;
                E[j << 1 | 1].cap <<= 1, E[j << 1 | 1].flow <<= 1;
            }
            for (int j = 0; j < m + 1; j++) {
                if (E[j << 1].raw_cap & (1LL << i)) {
                    Edge &e = E[j << 1];
                    if (e.flow != e.cap) {
                        e.cap++;
                        continue;
                    }
                    if (dijkstra(e.v, e.u)) {
                        if (V[e.u].d + cost(e) < 0) {
                            for (int x = j << 1; x != -1; x = V[E[x].u].prev) {
                                E[x].flow++, E[x ^ 1].flow--;
                            }
                        }
                    }
                    e.cap++;
                    ll fix = std::max(0LL, -cost(e)), maxd = 0;
                    for (int u = 0; u <= n; u++)
                        if (V[u].d != INF)
                            maxd = std::max(maxd, V[u].d);
                    for (int u = 0; u <= n; u++) {
                        if (V[u].d != INF) {
                            V[u].h += V[u].d;
                        } else {
                            V[u].h += maxd + fix;
                        }
                    }
                    dijkstra(n + 1, n + 1);
                    for (int u = 0; u <= n; u++)
                        V[u].h += V[u].d;
                }
            }
        }
        out_flow = E[m << 1 | 0].flow;
        out_cost = 0;
        for (int i = 0; i < m; i++)
            out_cost += E[i << 1].flow * E[i << 1].cost;
    }
};

int main() {
    int n, m, s, t;
    scanf("%d%d", &n, &m);
    Dijkstra dijkstra;
    dijkstra.initial(n);
    for (int i = 1, u, v; i <= m; i++) {
        long long cap, cost;
        scanf("%d%d%lld%lld", &u, &v, &cap, &cost);
        dijkstra.addEdge(u, v, cap, cost);
    }
    long long out_flow, out_cost;
    s = 1, t = n;
    dijkstra.successive_shortest_path(s, t, out_flow, out_cost);
    printf("%lld %lld\n", out_flow, out_cost);
    return 0;
}
