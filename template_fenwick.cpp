#include <cstring>
#include <iostream>
#include <vector>

struct Fenwick {
    typedef long long ll;
    static int lowbit(int x) {
        return x & -x;
    }

    Fenwick(int n) {
        C.resize(1 + n, 0);
    }

    std::vector<int> C;
    ll query(int x) {
        ll ret = 0;
        while (x > 0) {
            ret += C[x];
            x -= lowbit(x);
        }
        return ret;
    }
    void update(int x, ll delta) {
        while (x < C.size()) {
            C[x] += delta;
            x += lowbit(x);
        }
    }
};
