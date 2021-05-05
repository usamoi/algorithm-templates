#include <algorithm>
#include <iostream>
#include <queue>
#include <string>

template <typename string = std::string>
struct ACM {
    static const int SIGMA = 26;
    static int encode(char c) {
        return c - 'a';
    }
    struct Node {
        int ch[SIGMA] = {}, fail = 0;
    };
    std::vector<Node> V;
    std::vector<int> S;
    void initial() {
        V.clear(), V.push_back(Node());
        S.clear();
    }
    size_t size() {
        return V.size();
    }
    void insert(const string &s) {
        int u = 0;
        for (auto c : s) {
            if (!V[u].ch[encode(c)]) {
                V[u].ch[encode(c)] = V.size();
                V.push_back(Node());
            }
            u = V[u].ch[encode(c)];
        }
        S.push_back(u);
    }
    void build() {
        std::queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < SIGMA; i++) {
                if (V[u].ch[i]) {
                    if (u) {
                        V[V[u].ch[i]].fail = V[V[u].fail].ch[i];
                    }
                    q.push(V[u].ch[i]);
                } else {
                    V[u].ch[i] = V[V[u].fail].ch[i];
                }
            }
        }
    }
    template <typename Callback>
    void query(const string &s, Callback const &callback) {
        int u = 0;
        for (auto c : s) {
            u = V[u].ch[encode(c)];
            for (int v = u; v && callback(v); v = V[v].fail)
                ;
        }
    }
};



int main() {
    std::ios::sync_with_stdio(false);
    int n;
    ACM acm;
    while (std::cin >> n && n) {
        std::string input[n];
        {
            acm.initial();
            for (int i = 0; i < n; i++) {
                std::cin >> input[i];
                acm.insert(input[i]);
            }
            acm.build();
        }
        std::string s;
        std::cin >> s;
        {
            int size = acm.size();
            std::vector<int> cnt_v(size);
            std::vector<int> cnt_s(n);
            acm.query(s, [&](int v) {
                cnt_v[v]++;
                return true;
            });
            for (int i = 0; i < n; i++) {
                cnt_s[i] = cnt_v[acm.S[i]];
            }
            int ans = *std::max_element(cnt_s.begin(), cnt_s.end());
            std::cout << ans << std::endl;
            for (int i = 0; i < n; i++) {
                if (cnt_s[i] == ans) {
                    std::cout << input[i] << std::endl;
                }
            }
        }
    }
    return 0;
}