#include <cstdio>
#include <string>
#include <vector>

template <typename string = std::string>
void getNext(const string &T, std::vector<int> &next) {
    next = std::vector<int>(T.size() + 1, 0);

    for (int i = 1, j = 0; i < (int)T.size(); i++) {
        while (j && T[j] != T[i])
            j = next[j];
        j = (T[i] == T[j] ? j + 1 : 0);
        next[i + 1] = j;
    }
}

template <typename Callback, typename string = std::string>
void kmp(const string &T, const string &S, Callback callback) {
    std::vector<int> next;
    getNext(T, next);

    for (int i = 0, j = 0; i < (int)S.size(); i++) {
        while (j && S[i] != T[j])
            j = next[j];
        j = (S[i] == T[j] ? j + 1 : 0);
        if (j == (int)T.size())
            callback(i);
    }
}

int main() {
    std::vector<int> S, T;
    int Tcas, n, m;
    scanf("%d", &Tcas);
    for (int cas = 1; cas <= Tcas; cas++) {
        scanf("%d%d", &n, &m);
        S.resize(n);
        T.resize(m);
        for (int i = 0; i < n; i++) {
            scanf("%d", &S[i]);
        }
        for (int i = 0; i < m; i++) {
            scanf("%d", &T[i]);
        }
        int ans = -1;
        kmp(T, S, [&ans, m](int i) { if(ans == -1)ans = i - m + 1 + 1; });
        printf("%d\n", ans);
    }
    return 0;
}
