#include <algorithm>
#include <cstdio>
#include <map>
#include <vector>

struct TreeDivide {
    typedef long long ll;
    struct Edge {
        int u, v;
        ll weight;
    };
    struct Vertex {
        bool solved = false;
        int sum = 0;
        int fa, heavyson;
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
    template <typename Fun>
    void travel(int u, Fun f) {
        for (auto index : V[u].to) {
            auto &e = E[index];
            if (V[e.v].solved || V[e.u].fa == e.v)
                continue;
            f(e);
        }
    }
    void maintain(int u, int fa) {
        if (V[u].fa == fa)
            return;
        V[u].fa = fa, V[u].heavyson = -1, V[u].sum = 1;
        travel(u, [&](Edge &e) {
            maintain(e.v, u);
            V[u].sum += V[e.v].sum;
            if (V[u].heavyson == -1 || V[V[u].heavyson].sum < V[e.v].sum)
                V[u].heavyson = e.v;
        });
    }
    template <typename Callback>
    void divide(int u, Callback solve) {
        int vsize = V[u].sum;
        while (V[u].heavyson != -1 && V[V[u].heavyson].sum > vsize - V[u].sum && V[V[u].heavyson].sum > vsize / 2)
            u = V[u].heavyson;
        maintain(u, -1), solve(u), V[u].solved = true;
        travel(u, [&](Edge &e) {
            divide(e.v, solve);
        });
    }
    template <typename Callback>
    void divide(Callback solve) {
        int u = 1;
        if (V[u].sum == 0)
            maintain(u, u), V[u].fa = -1;
        divide(u, solve);
    }
};
