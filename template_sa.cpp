// author  : Menci (modified)
// source  : https://oi.men.ci/suffix-array-notes/
// license : CC BY-NC-SA 4.0

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

template <typename string = std::string>
void suffixArray(const string &oris, std::vector<int>& sa, std::vector<int>& rk) {
    int n = oris.size();

    sa = rk = std::vector<int>(n);
    std::vector<int> a(n), fir(1 + n), sec(1 + n), tmp(1 + n), buc(1 + n);
    {
        string charset = oris;
        std::sort(charset.begin(), charset.end());
        auto end = std::unique(charset.begin(), charset.end());
        for (int i = 0; i < n; i++)
            a[i] = std::lower_bound(charset.begin(), end, oris[i]) - charset.begin();
    }
    
    for (int i = 0; i < n; i++)
        buc[a[i] + 1]++;
    for (int i = 1; i <= n; i++)
        buc[i] += buc[i - 1];
    for (int i = 0; i < n; i++)
        rk[i] = buc[a[i]] + 1;

    for (int t = 1; t <= n; t *= 2) {
        for (int i = 1; i <= n; i++)
            fir[i] = rk[i - 1];
        for (int i = 1; i <= n; i++)
            sec[i] = i + t > n ? 0 : rk[i + t - 1];

        std::fill(buc.begin(), buc.end(), 0);
        for (int i = 1; i <= n; i++)
            buc[sec[i]]++;
        for (int i = 1; i <= n; i++)
            buc[i] += buc[i - 1];
        for (int i = 1; i <= n; i++)
            tmp[n - --buc[sec[i]]] = i;

        std::fill(buc.begin(), buc.end(), 0);
        for (int i = 1; i <= n; i++)
            buc[fir[i]]++;
        for (int i = 1; i <= n; i++)
            buc[i] += buc[i - 1];
        for (int j = 1, i; j <= n; j++)
            i = tmp[j], sa[--buc[fir[i]]] = i;

        bool unique = true;
        for (int j = 0, i, last = 0; j < n; j++) {
            i = sa[j];
            if (!last)
                rk[i - 1] = 1;
            else if (fir[i] == fir[last] && sec[i] == sec[last])
                rk[i - 1] = rk[last - 1], unique = false;
            else
                rk[i - 1] = rk[last - 1] + 1;

            last = i;
        }
        if (unique)
            break;
    }
}

int main() {
    std::vector<int> sa, rk;
    std::string s;
    std::cin >> s;
    suffixArray(s, sa, rk);
    for (int i = 0; i < (int)s.size(); i++) {
        std::cout << sa[i] << ' ';
    }
    return 0;
}
