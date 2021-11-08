// author  : Menci (modified)
// source  : https://oi.men.ci/suffix-array-notes/
// license : CC BY-NC-SA 4.0

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace SA {
    typedef std::vector<int> Vec;
    template <typename string = std::string>
    Vec discrete(const string &str) {
        int n = str.size();
        Vec s(1 + n);
        std::string charset = str;
        std::sort(charset.begin(), charset.end());
        charset.erase(std::unique(charset.begin(), charset.end()), charset.end());
        for (int i = 1; i <= n; i++)
            s[i] = std::lower_bound(charset.begin(), charset.end(), str[i - 1]) - charset.begin() + 1;
        return s;
    }
    std::pair<Vec, Vec> construct(const Vec &s) {
        int n = s.size() - 1;
        Vec sa(1 + n), rk(1 + n);
        Vec fir(1 + n), sec(1 + n), tmp(1 + n), buc(1 + n);
        for (int i = 1; i <= n; i++)
            buc[s[i]]++;
        for (int i = 1; i <= n; i++)
            buc[i] += buc[i - 1];
        for (int i = 1; i <= n; i++)
            rk[i] = buc[s[i] - 1] + 1;
        for (int t = 1; t <= n; t *= 2) {
            for (int i = 1; i <= n; i++)
                fir[i] = rk[i];
            for (int i = 1; i <= n; i++)
                sec[i] = i + t > n ? 0 : rk[i + t];
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
                i = tmp[j], sa[buc[fir[i]]--] = i;
            bool unique = true;
            for (int j = 1, i, last = 0; j <= n; j++) {
                i = sa[j];
                if (!last)
                    rk[i] = 1;
                else if (fir[i] == fir[last] && sec[i] == sec[last])
                    rk[i] = rk[last], unique = false;
                else
                    rk[i] = rk[last] + 1;
                last = i;
            }
            if (unique)
                break;
        }
        return std::make_pair(sa, rk);
    }
    Vec getHeight(const Vec &s, const Vec &sa, const Vec &rk) {
        int n = s.size() - 1;
        Vec ht(1 + n);
        for (int i = 1, k = 0; i <= n; ++i) {
            k = std::max(0, k - 1);
            while (s[i + k] == s[sa[rk[i] - 1] + k])
                k++;
            ht[rk[i]] = k;
        }
        return ht;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string str;
    std::cin >> str;
    auto s = SA::discrete(str);
    auto [sa, rk] = SA::construct(s);
    auto ht = SA::getHeight(s, sa, rk);
    for (int i = 1; i <= (int)str.size(); i++) {
        std::cout << "{ idx = " << i << ", sa = " << sa[i] << ", ht = " << ht[i] << " }" << std::endl;
    }
    return 0;
}
