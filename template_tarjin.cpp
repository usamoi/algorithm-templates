#include <cstdio>
#include <stack>
#include <vector>

struct Tarjin {
    int n, dfsclock, scccnt;
    struct Vertex {
        std::vector<int> e;
        int index, lowlink, sccno;
    };
    struct Edge {
        int u, v;
    };
    std::vector<Vertex> V;
    std::vector<Edge> E;
    void initial(int _n) {
        n = _n;
        V.clear(), V.resize(1 + n);
        E.clear();
    }
    void addEdge(int u, int v) {
        V[u].e.push_back(E.size()), E.push_back(Edge{u, v});
    }
    void tarjanDfs(std::stack<int> &s, int u) {
        V[u].lowlink = V[u].index = ++dfsclock;
        s.push(u);
        for (auto ei : V[u].e) {
            int v = E[ei].v;
            if (!V[v].index) {
                tarjanDfs(s, v);
                V[u].lowlink = std::min(V[u].lowlink, V[v].lowlink);
            } else if (!V[v].sccno) {
                V[u].lowlink = std::min(V[u].lowlink, V[v].index);
            }
        }
        if (V[u].lowlink != V[u].index)
            return;
        int sccno = ++scccnt;
        while (s.size() && s.top() != u) {
            int v = s.top();
            s.pop();
            V[v].sccno = sccno;
        }
    }
    void tarjan() {
        dfsclock = 0, scccnt = 0;
        std::stack<int> s;
        for (int i = 0; i < (int)V.size(); i++)
            V[i].index = 0;
        for (int i = 0; i < (int)V.size(); i++)
            if (!V[i].index)
                tarjanDfs(s, i);
    }
} g;

int main() {
    return 0;
}