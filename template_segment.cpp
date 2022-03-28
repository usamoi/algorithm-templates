#include <functional>
#include <iostream>
#include <limits>
#include <optional>

using ll = long long;

struct Value {
    ll x = 0;
    Value operator*(Value rhs) const {
        return Value{x + rhs.x};
    }
};

struct Delta {
    ll d = 0;
    Delta operator*(Delta rhs) const {
        return Delta{d + rhs.d};
    }
};

Value make(Value lhs, Delta rhs, int l, int r) {
    return Value{lhs.x + rhs.d * (r - l)};
}

struct Node {
    int l, r;
    Node *L, *R;
    Value x;
    Delta d;
    void pushdown() {
        if (L) {
            L->d = L->d * d, L->x = make(L->x, d, L->l, L->r);
            R->d = R->d * d, R->x = make(R->x, d, R->l, R->r);
        }
        d = Delta();
    }
    void pushup() {
        if (L) {
            x = L->x * R->x;
        }
    }
};

template <typename F>
Node *build(int l, int r, F f) {
    if (l == r) {
        return nullptr;
    }
    if (l + 1 == r) {
        return new Node{l, r, nullptr, nullptr, f(l)};
    }
    int m = l + ((r - l + 1) >> 1);
    auto u = new Node{l, r, build(l, m, f), build(m, r, f)};
    u->pushup();
    return u;
}

Value query(Node *u, int l, int r) {
    if (!u || u->r <= l || r <= u->l) {
        return Value();
    }
    if (l <= u->l && u->r <= r) {
        return u->x;
    }
    u->pushdown();
    return query(u->L, l, r) * query(u->R, l, r);
}

void update(Node *u, int l, int r, Delta d) {
    if (!u || u->r <= l || r <= u->l) {
        return;
    }
    if (l <= u->l && u->r <= r) {
        u->d = u->d * d, u->x = make(u->x, d, u->l, u->r);
        return;
    }
    u->pushdown();
    update(u->L, l, r, d);
    update(u->R, l, r, d);
    u->pushup();
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<int> s(n);
    for (int i = 0; i < n; i++) {
        std::cin >> s[i];
    }
    auto root = build(1, n + 1, [&](int i) { return Value{s[i - 1]}; });
    for (int i = 1; i <= m; i++) {
        int op;
        std::cin >> op;
        if (op == 1) {
            int l, r;
            ll d;
            std::cin >> l >> r >> d;
            update(root, l, r + 1, Delta{d});
        }
        if (op == 2) {
            int l, r;
            std::cin >> l >> r;
            std::cout << query(root, l, r + 1).x << "\n";
        }
    }
    return 0;
}
