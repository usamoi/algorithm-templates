#include <algorithm>
#include <cstdio>
#include <queue>
#include <vector>

struct Dijkstra {
    typedef long long ll;
    static const ll INF = 1LL << 50;
    struct Edge {
        int u, v;
        ll w;
    };
    struct Vertex {
        ll d = INF;
        std::vector<int> to;
    };
    std::vector<Edge> E;
    std::vector<Vertex> V;
    void initial(int n) {
        V.clear(), V.resize(1 + n);
        E.clear();
    }
    void addEdge(int u, int v, ll w) {
        V[u].to.push_back(E.size());
        E.push_back(Edge{u, v, w});
    }
    ll dijkstra(int S, int T, ll pe) {
        for(auto& u : V){
            u.d = INF;
        }
        typedef std::pair<ll, int> Node;
        std::priority_queue<Node> q;
        q.push(Node{V[S].d = 0, S});
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (d != V[u].d)
                continue;
            for (auto index : V[u].to) {
                Edge &e = E[index];
                if (V[e.v].d > V[e.u].d + e.w)
                    q.push(Node{V[e.v].d = V[e.u].d + e.w, e.v});
            }
        }
        return V[T].d;
    }
};
