#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

struct ST {
    std::vector<std::vector<int>> table;
    void initial(const std::vector<int> &ori) {
        int n = ori.size(), height = log(n) / log(2) + 1;
        table.resize(height, std::vector<int>(n)), table[0] = ori;

        for (int k = 1; k < height; k++)
            for (int i = 0; i + (1 << k) <= n; i++)
                table[k][i] = std::max(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
    }
    int query(int l, int r) {
        int k = log(r - l + 1) / log(2);
        return std::max(table[k][l], table[k][r + 1 - (1 << k)]);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    ST st;
    int n, Q;
    std::cin >> n >> Q;
    std::vector<int> ori;
    for (int i = 0, x; i < n; i++)
        std::cin >> x, ori.push_back(x);
    st.initial(ori);
    while (Q--) {
        int l, r;
        std::cin >> l >> r, l--, r--;
        std::cout << st.query(l, r) << std::endl;
    }
    return 0;
}
