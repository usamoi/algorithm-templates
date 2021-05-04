#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>

namespace Healight {
    struct Node {
        std::vector<Node *> v;
        Node *son, *fa, *top;
        int size, depth, code = 0;

        void build(Node *tfa = nullptr) {
            fa = tfa;
            depth = fa ? (fa->depth + 1) : 1;
            size = 1;
            son = 0;
            for (auto node : v) {
                if (node == fa)
                    continue;
                node->build(this);
                size += node->size;
                if (son == 0 || node->size > son->size)
                    son = node;
            }
        }

        void generate() {
            static int dfs_clock = 0;
            code = ++dfs_clock;
            top = fa && fa->son == this ? fa->top : this;
            if (son)
                son->generate();
            for (auto node : v) {
                if (node == fa || node == son)
                    continue;
                node->generate();
            }
        }
    };

    Node *LCA(Node *x, Node *y) {
        while (x->top != y->top) {
            if (x->top->depth > y->top->depth)
                std::swap(x, y);
            y = y->top->fa;
        }
        return x->depth < y->depth ? x : y;
    }

    Node* make(int n){
        return new Node[1 + n];
    }
} // namespace Healight

int main() {
    int N, M, S, x, y;
    std::cin >> N >> M >> S;
    auto healight = Healight::make(N);
    for (int i = 1; i < N; i++) {
        std::cin >> x >> y;
        healight[x].v.push_back(&healight[y]);
        healight[y].v.push_back(&healight[x]);
    }
    healight[S].build();
    healight[S].generate();
    for (int i = 1; i <= M; i++) {
        std::cin >> x >> y;
        std::cout << LCA(&healight[x], &healight[y]) - &healight[0] << std::endl;
    }
    return 0;
}