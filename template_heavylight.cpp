#include <algorithm>
#include <cstdio>
#include <vector>

struct Heavylight {
    struct Node {
        std::vector<Node *> to;
        Node *son, *fa, *top;
        int size, depth, code;
        void build(Node *_fa = nullptr) {
            fa = _fa;
            depth = fa ? (fa->depth + 1) : 1;
            size = 1;
            son = 0;
            for (auto v : to) {
                if (v == fa)
                    continue;
                v->build(this);
                size += v->size;
                if (son == 0 || v->size > son->size)
                    son = v;
            }
        }
        void generate(int &dfs_clock) {
            code = ++dfs_clock;
            top = fa && fa->son == this ? fa->top : this;
            if (son)
                son->generate(dfs_clock);
            for (auto node : to) {
                if (node == fa || node == son)
                    continue;
                node->generate(dfs_clock);
            }
        }
    };
    std::vector<Node> V;
    void initial(int n) {
        V.clear(), V.resize(1 + n);
    }
    void addEdge(int u, int v) {
        V[u].to.push_back(&V[v]);
        V[v].to.push_back(&V[u]);
    }
    void complete(int s) {
        int dfs_clock = 0;
        V[s].build();
        V[s].generate(dfs_clock);
    }
    static Node *LCA(Node *x, Node *y) {
        while (x->top != y->top) {
            if (x->top->depth > y->top->depth)
                std::swap(x, y);
            y = y->top->fa;
        }
        return x->depth < y->depth ? x : y;
    }
};

int main() {
    int n, m, s;
    scanf("%d%d%d", &n, &m, &s);
    Heavylight healight;
    healight.initial(n);
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        healight.addEdge(u, v);
    }
    healight.complete(s);
    for (int i = 1, u, v; i <= m; i++) {
        scanf("%d%d", &u, &v);
        printf("%d\n", (int)(Heavylight::LCA(&healight.V[u], &healight.V[v]) - &healight.V[0]));
    }
    return 0;
}
