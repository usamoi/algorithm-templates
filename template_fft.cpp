#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

namespace FFT {
    typedef std::complex<double> cn;
    const double PI = 3.14159265358979323846;

    void FFT(std::vector<cn> &a, int on) {
        int n = a.size();
        for (int i = 0, j = 0; i < n; i++) {
            if (j > i)
                std::swap(a[i], a[j]);
            int k = n;
            while (j & (k >>= 1))
                j &= ~k;
            j |= k;
        }
        for (int s = 1; s < n; s <<= 1) {
            for (int k = 0; k < s; k++) {
                double arc = PI * on / s * k;
                cn w = cn(cos(arc), sin(arc));
                for (int Ek = k; Ek < n; Ek += s << 1) {
                    int Ok = Ek + s;
                    cn t = w * a[Ok];
                    a[Ok] = a[Ek] - t;
                    a[Ek] += t;
                }
            }
        }
        if (on == -1)
            for (int i = 0; i < n; i++)
                a[i] /= n;
    }

    std::vector<double> operator*(const std::vector<double> &v1, const std::vector<double> &v2) {
        int s1 = v1.size(), s2 = v2.size(), S = 2, RES = s1 + s2 - 1;
        while (S <= RES)
            S <<= 1;
        std::vector<cn> a(S), b(S);
        std::copy(v1.begin(), v1.end(), a.begin()), FFT(a, 1);
        std::copy(v2.begin(), v2.end(), b.begin()), FFT(b, 1);
        for (int i = 0; i < S; i++)
            a[i] *= b[i];
        FFT(a, -1);
        std::vector<double> res(RES);
        for (int i = 0; i < RES; i++)
            res[i] = a[i].real();
        return res;
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<double> a(n + 1), b(m + 1);
    for (int i = 0; i <= n; i++) {
        std::cin >> a[i];
    }
    for (int i = 0; i <= m; i++) {
        std::cin >> b[i];
    }
    std::vector<double> c = FFT::operator*(a, b);
    for (int i = 0; i < (int)c.size(); i++) {
        std::cout << (int)(c[i] + 0.5) << ' ';
    }
    return 0;
}