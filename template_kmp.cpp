#include <iostream>
#include <string>
#include <vector>

template <typename string = std::string>
void getNext(const string &T, std::vector<int> &next) {
    next = std::vector<int>(T.size() + 1, 0);

    for (int i = 1, j = 0; i < T.size(); i++) {
        while (j && T[j] != T[i])
            j = next[j];
        j = (T[i] == T[j] ? j + 1 : 0);
        next[i + 1] = j;
    }
}

template <typename Callback, typename string = std::string>
void match(const string &T, const string &S, Callback const &callback) {
    std::vector<int> next;
    getNext(T, next);

    for (int i = 0, j = 0; i < S.size(); i++) {
        while (j && S[i] != T[j])
            j = next[j];
        j = (S[i] == T[j] ? j + 1 : 0);
        if (j == T.size())
            callback(i);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::vector<int> S, T;
    int Tcas, n, m;
    std::cin >> Tcas;
    for (int cas = 1; cas <= Tcas; cas++) {
        std::cin >> n >> m;
        S.resize(n);
        T.resize(m);
        for (int i = 0; i < n; i++) {
            std::cin >> S[i];
        }
        for (int i = 0; i < m; i++) {
            std::cin >> T[i];
        }
        int ans = -1;
        match(T, S, [&ans, m](int i) { if(ans == -1)ans = i - m + 1 + 1; });
        std::cout << ans << std::endl;
    }
    return 0;
}