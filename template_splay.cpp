#include <algorithm>
#include <iostream>

namespace Splay {
    typedef long long ll;
    struct Node {
        ll value;
        int cnt = 0, size;
        Node *fa = nullptr, *ch[2] = {};
        void pushup() {
            size = cnt + (ch[0] ? ch[0]->size : 0) + (ch[1] ? ch[1]->size : 0);
        }
        Node(ll value) : value(value) {
            pushup();
        }
        int type() {
            return fa ? (fa->ch[0] == this ? 0 : 1) : -1;
        }
        static void adopt(Node *x, Node *y, int z) {
            if (x && (z != -1))
                x->ch[z] = y;
            if (y)
                y->fa = x;
        }
        void rotate() {
            auto fa = this->fa;
            if (!fa)
                return;
            int d = type();
            adopt(fa->fa, this, fa->type());
            adopt(fa, ch[d ^ 1], d);
            adopt(this, fa, d ^ 1);
            fa->pushup();
            pushup();
        }
        void splay(const Node *goal = nullptr) {
            while (fa != goal) {
                if (fa->fa != goal)
                    (type() == fa->type() ? fa : this)->rotate();
                rotate();
            }
        }
        Node *extremum(int d) {
            auto x = this;
            while (x->ch[d])
                x = x->ch[d];
            return x;
        }
    };
    struct Set {
        void splay(Node *x) {
            if (x) {
                x->splay();
                root = x;
            }
        }
        Node *root = nullptr;
        std::pair<bool, Node *> search(ll value) {
            Node *last = nullptr, *x = root;
            while (x) {
                if (value == x->value)
                    return std::make_pair(true, x);
                x = (last = x)->ch[value < x->value ? 0 : 1];
            }
            return std::make_pair(false, last);
        }
        Node *insert(ll value) {
            auto f = search(value);
            auto x = f.first ? f.second : new Node(value);
            if (!f.first && root) {
                Node::adopt(f.second, x, value < f.second->value ? 0 : 1);
            }
            splay(x);
            root->cnt++, root->pushup();
            return root;
        }
        int count(ll value) {
            auto f = search(value);
            splay(f.second);
            return f.first ? f.second->cnt : 0;
        }
        bool erase(ll value) {
            if (!count(value))
                return false;
            if (root->cnt > 1) {
                root->cnt--, root->pushup();
                return true;
            }
            auto dead = root;
            if (!dead->ch[0]) {
                dead->ch[1]->fa = nullptr;
                root = dead->ch[1];
                if (root)
                    root->pushup();
            } else {
                dead->ch[0]->extremum(1)->splay(dead);
                Node::adopt(dead->ch[0], dead->ch[1], 1);
                dead->ch[0]->fa = nullptr;
                root = dead->ch[0], root->pushup();
            }
            delete dead;
            return true;
        }
        Node *placeof(int kth) {
            if (kth < 1 || root->size < kth)
                return nullptr;
            for (auto x = root;;) {
                int left = x->ch[0] ? x->ch[0]->size : 0, mid = x -> cnt;
                if (kth <= left) {
                    x = x->ch[0];
                    continue;
                }
                if (kth <= left + mid) {
                    splay(x);
                    return x;
                }
                kth -= left + mid, x = x->ch[1];
            }
        }
        int rankof(ll value) {
            if (!root)
                return 0;
            count(value);
            int ans = 0;
            ans += root->ch[0] ? root->ch[0]->size : 0;
            ans += value > root->value ? root->cnt : 0;
            return ans + 1;
        }
    };
}

int main() {
    std::ios::sync_with_stdio(false);
    int n, opt, x;
    Splay::Set S;
    std::cin >> n;
    while (n--) {
        std::cin >> opt >> x;
        if (opt == 1)
            S.insert(x);
        if (opt == 2)
            S.erase(x);
        if (opt == 3)
            std::cout << S.rankof(x) << std::endl;
        if (opt == 4)
            std::cout << S.placeof(x)->value << std::endl;
        if (opt == 5)
            S.count(x), std::cout << (S.root->value < x ? S.root : S.root->ch[0]->extremum(1))->value << std::endl;
        if (opt == 6)
            S.count(x), std::cout << (S.root->value > x ? S.root : S.root->ch[1]->extremum(0))->value << std::endl;
    }
    return 0;
}