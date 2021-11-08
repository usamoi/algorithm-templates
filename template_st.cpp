#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

struct ST {
    typedef int ll;
    std::vector<std::vector<ll>> t;
    void initial(const std::vector<ll> &a) {
        int height = std::log2(a.size()) + 1;
        t = std::vector(height, std::vector<ll>(a.size())), t[0] = a;
        for (int k = 1; k < height; k++)
            for (int i = 0; i + (1 << k) <= (int)a.size(); i++)
                t[k][i] = std::max(t[k - 1][i], t[k - 1][i + (1 << (k - 1))]);
    }
    ll query(int l, int r) {
        int k = std::log2(r - l + 1);
        return std::max(t[k][l], t[k][r + 1 - (1 << k)]);
    }
};

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    std::vector<int> a;
    for (int i = 0, x; i < n; i++) {
        scanf("%d", &x);
        a.push_back(x);
    }
    ST st;
    st.initial(a);
    for (int i = 1, l, r; i <= q; i++) {
        scanf("%d%d", &l, &r), l--, r--;
        printf("%d\n", st.query(l, r));
    }
    return 0;
}
