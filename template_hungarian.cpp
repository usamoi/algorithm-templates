// author: OI Wiki
// source: https://oi-wiki.org/graph/graph-matching/bigraph-weight-match/
// license : CC BY-SA 4.0 + SATA

#include <algorithm>
#include <cstdio>
#include <queue>
#include <vector>

struct Hungarian {
    typedef long long ll;
    const ll INF = 1e12; // `static` makes the linker puzzled
    std::vector<std::vector<ll>> weights;
    int n;
    void initial(int _n) {
        n = _n;
        weights = std::vector<std::vector<ll>>(n, std::vector<ll>(n, -INF));
    }
    void addEdge(int x, int y, ll weight) {
        weights[x][y] = std::max(weight, weights[x][y]);
    }
    std::vector<int> match_x, match_y;
    std::vector<ll> l_x, l_y;
    void bfs(int x) {
        auto vis_x = std::vector<bool>(n, false);
        auto vis_y = std::vector<bool>(n, false);
        auto slack = std::vector<ll>(n, INF);
        auto prev = std::vector<int>(n, 0);
        auto queue = std::queue<int>();
        auto check = [&](int y) {
            vis_y[y] = true;
            if (match_y[y] != -1) {
                queue.push(match_y[y]);
                vis_x[match_y[y]] = true;
                return false;
            }
            while (y != -1) {
                match_y[y] = prev[y];
                std::swap(y, match_x[prev[y]]);
            }
            return true;
        };
        queue.push(x), vis_x[x] = true;
        while (true) {
            while (!queue.empty()) {
                int x = queue.front();
                queue.pop();
                for (int y = 0; y < n; y++) {
                    if (vis_y[y]) {
                        continue;
                    }
                    ll delta = l_x[x] + l_y[y] - weights[x][y];
                    if (delta <= slack[y]) {
                        prev[y] = x;
                        if (delta != 0) {
                            slack[y] = delta;
                        }
                        if (delta == 0 && check(y)) {
                            return;
                        }
                    }
                }
            }
            ll d = INF;
            for (int y = 0; y < n; y++) {
                if (!vis_y[y]) {
                    d = std::min(d, slack[y]);
                }
            }
            for (int x = 0; x < n; x++) {
                if (vis_x[x]) {
                    l_x[x] -= d;
                }
            }
            for (int y = 0; y < n; y++) {
                if (vis_y[y]) {
                    l_y[y] += d;
                }
                if (!vis_y[y]) {
                    slack[y] -= d;
                }
            }
            for (int y = 0; y < n; y++) {
                if (!vis_y[y] && slack[y] == 0 && check(y)) {
                    return;
                }
            }
        }
    }
    ll hungarian() {
        match_x = std::vector<int>(n, -1);
        match_y = std::vector<int>(n, -1);
        l_x = std::vector<ll>(n);
        for (int x = 0; x < n; x++) {
            l_x[x] = *std::max_element(weights[x].begin(), weights[x].end());
        }
        l_y = std::vector<ll>(n, 0);
        for (int x = 0; x < n; x++) {
            bfs(x);
        }
        ll ans = 0;
        for (int x = 0; x < n; x++) {
            ans += weights[x][match_x[x]];
        }
        return ans;
    }
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    Hungarian h;
    h.initial(n);
    for (int i = 0, x, y, w; i < m; i++) {
        scanf("%d%d%d", &x, &y, &w);
        h.addEdge(x - 1, y - 1, w);
    }
    long long ans = h.hungarian();
    printf("%lld\n", ans);
    for (int y = 1; y <= n; y++) {
        printf("%d ", h.match_y[y - 1] + 1);
    }
}
