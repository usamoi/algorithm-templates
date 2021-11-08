#include <cstdio>
#include <stack>
#include <vector>

struct TwoSat {
    struct Vertex {
        std::vector<int> e;
        int index = 0, lowlink, sccno;
    };
    struct Edge {
        int u, v;
    };
    int n, dfsclock, scccnt;
    std::vector<Vertex> V;
    std::vector<Edge> E;
    void initial(int _n) {
        n = _n;
        V.clear(), V.resize(2 * n);
        E.clear();
        dfsclock = 0, scccnt = 0;
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
        while (true) {
            int v = s.top();
            s.pop();
            V[v].sccno = sccno;
            if (u == v)
                break;
        }
    }
    std::vector<int> solve() {
        std::stack<int> s;
        for (int i = 0; i < 2 * n; i++)
            if (!V[i].index)
                tarjanDfs(s, i);
        std::vector<int> ans;
        for (int x = 0; x < n; x++)
            if (V[2 * x + 0].sccno == V[2 * x + 1].sccno)
                return ans;
        for (int x = 0; x < n; x++) {
            if (V[2 * x + 0].sccno < V[2 * x + 1].sccno)
                ans.push_back(0);
            else
                ans.push_back(1);
        }
        return ans;
    }
} ts;

int main() {
    int k, n;
    scanf("%d%d", &k, &n);
    ts.initial(k);
    for (int i = 0; i < n; i++) {
        int p1, p2, p3;
        char c1, c2, c3;
        scanf("%d %c %d %c %d %c", &p1, &c1, &p2, &c2, &p3, &c3);
        int r1 = c1 == 'R' ? 0 : 1, r2 = c2 == 'R' ? 0 : 1, r3 = c3 == 'R' ? 0 : 1;
        p1--, p2--, p3--;
        //1..2
        ts.addEdge(p1 * 2 + (r1 ^ 1), p2 * 2 + r2);
        ts.addEdge(p2 * 2 + (r2 ^ 1), p1 * 2 + r1);
        //1..3
        ts.addEdge(p1 * 2 + (r1 ^ 1), p3 * 2 + r3);
        ts.addEdge(p3 * 2 + (r3 ^ 1), p1 * 2 + r1);
        //2..3
        ts.addEdge(p2 * 2 + (r2 ^ 1), p3 * 2 + r3);
        ts.addEdge(p3 * 2 + (r3 ^ 1), p2 * 2 + r2);
    }
    std::vector<int> ans = ts.solve();
    if (!ans.size()) {
        printf("-1");
    } else {
        for (auto val : ans) {
            printf("%c", val == 0 ? 'R' : 'B');
        }
    }
    return 0;
}
