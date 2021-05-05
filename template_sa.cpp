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
    void sadc(const string &s, Vec &a) {
        int n = s.size();
        a = Vec(1 + n);
        std::string charset = s;
        std::sort(charset.begin(), charset.end());
        charset.erase(std::unique(charset.begin(), charset.end()), charset.end());
        for (int i = 1; i <= n; i++)
            a[i] = std::lower_bound(charset.begin(), charset.end(), s[i - 1]) - charset.begin() + 1;
    }

    void sark(const Vec &a, Vec &sa, Vec &rk) {
        int n = a.size() - 1;

        sa = rk = Vec(1 + n);
        Vec fir(1 + n), sec(1 + n), tmp(1 + n), buc(1 + n);

        for (int i = 1; i <= n; i++)
            buc[a[i]]++;
        for (int i = 1; i <= n; i++)
            buc[i] += buc[i - 1];
        for (int i = 1; i <= n; i++)
            rk[i] = buc[a[i] - 1] + 1;

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
    }

    void saht(const Vec &a, const Vec &sa, const Vec &rk, Vec &ht) {
        int n = a.size() - 1;
        ht = Vec(1 + n);
        for (int i = 1, k = 0; i <= n; ++i) {
            k = std::max(0, k - 1);
            while (a[i + k] == a[sa[rk[i] - 1] + k])
                k++;
            ht[rk[i]] = k;
        }
    }
} // namespace SA

int main() {
    SA::Vec a, sa, rk, ht;
    std::string s;
    std::cin >> s;
    int n = s.size();
    SA::sadc(s, a);
    SA::sark(a, sa, rk);
    SA::saht(a, sa, rk, ht);
    for (int i = 1; i <= n; i++) {
        std::cout << "{ idx = " << i << ", sa = " << sa[i] << ", ht = " << ht[i] << " }" << std::endl;
    }
    return 0;
}
