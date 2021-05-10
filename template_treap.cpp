#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace Treap {
    typedef long long ll;
    struct Node;
    typedef const Node *Imp;
    struct Node {
        ll value;
        int cnt = 0, size, weight;
        Imp L = nullptr, R = nullptr;
        void pushup() {
            size = cnt + (L ? L->size : 0) + (R ? R->size : 0);
        }
        Node(ll value) : value(value) {
            weight = rand();
            pushup();
        }
        Node *clone() const {
            auto n = (Node *)::operator new(sizeof(Node));
            *n = *this;
            return n;
        }
    };
    std::pair<Imp, Imp> split(Imp u, ll x) {
        if (u == nullptr) {
            return std::make_pair(nullptr, nullptr);
        } else {
            auto n = u->clone();
            if (x < u->value) {
                auto o = split(u->L, x);
                n->L = o.second;
                n->pushup();
                return std::make_pair(o.first, n);
            } else {
                auto o = split(u->R, x);
                n->R = o.first;
                n->pushup();
                return std::make_pair(n, o.second);
            }
        }
    }
    Imp merge(Imp u, Imp v) {
        if (!(u && v)) {
            return u ? u : v;
        }
        if (u->weight > v->weight) {
            auto n = u->clone();
            n->R = merge(u->R, v);
            n->pushup();
            return n;
        } else {
            auto n = v->clone();
            n->L = merge(u, v->L);
            n->pushup();
            return n;
        }
    }
    namespace Set {
        typedef Imp ImmutSet;
        ImmutSet find(ImmutSet u, ll x) {
            while (u && x != u->value) {
                u = x < u->value ? u->L : u->R;
            }
            return u;
        }
        ImmutSet insert(ImmutSet root, ll value) {
            auto o = split(root, value);
            auto s = split(o.first, value - 1);
            Node *n;
            if (s.second) {
                n = s.second->clone();
            } else {
                n = new Node(value);
            }
            n->cnt++, n->pushup();
            return merge(merge(s.first, n), o.second);
        }
        ImmutSet erase(ImmutSet root, ll value) {
            auto o = split(root, value);
            auto s = split(o.first, value - 1);
            Node *n;
            if (s.second) {
                n = s.second->clone();
                n->cnt--, n->pushup();
                if (n->cnt == 0) {
                    delete n;
                    n = nullptr;
                }
            } else {
                n = nullptr;
            }
            return merge(merge(s.first, n), o.second);
        }
        ImmutSet atrank(ImmutSet root, int kth) {
            ImmutSet x = root;
            if (!(1 <= kth && kth <= x->size)) {
                return nullptr;
            }
            while (true) {
                int left = x->L ? x->L->size : 0;
                if (kth <= left) {
                    x = x->L;
                    continue;
                }
                kth -= left;
                int mid = x->cnt;
                if (kth <= mid) {
                    return x;
                }
                kth -= mid;
                x = x->R;
            }
        }
    }; // namespace Set
} // namespace Treap

int main() {
    std::ios::sync_with_stdio(false);
    int n, opt, ver;
    Treap::ll val;
    std::vector<Treap::Set::ImmutSet> history(1);

    std::cin >> n;
    for (int cas = 1; cas <= n; cas++) {
        std::cin >> ver >> opt >> val;

        auto S = history[ver];
        switch (opt) {
        case 1:
            S = Treap::Set::insert(S, val);
            break;

        case 2:
            S = Treap::Set::erase(S, val);
            break;

        case 3: {
            auto o = Treap::split(S, val - 1);
            int ans = (o.first ? o.first->size : 0) + 1;
            std::cout << ans << std::endl;
        } break;

        case 4:
            std::cout << Treap::Set::atrank(S, val)->value << std::endl;
            break;

        case 5: {
            auto o = Treap::split(S, val - 1).first;
            while (o->R)
                o = o->R;
            std::cout << o->value << std::endl;
        } break;

        case 6: {
            auto o = Treap::split(S, val).second;
            while (o->L)
                o = o->L;
            std::cout << o->value << std::endl;
        } break;
        }

        history.push_back(S);
    }
}