#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace NTT {
    typedef long long ll;
    const ll P = 998244353LL; // (2^23)*7*17+1

    ll fastpow(ll a, ll b, ll m) {
        ll ans = 1;
        a %= m;
        while (b) {
            if (b & 1)
                ans = ans * a % m;
            a = a * a % m, b >>= 1;
        }
        return ans;
    }

    void NTT(std::vector<ll> &a, int on) {
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
            ll wn = fastpow(3, (P - 1) / (s << 1), P), w = 1;
            if (on == -1)
                wn = fastpow(wn, P - 2, P);
            for (int k = 0; k < s; k++) {
                for (int Ek = k; Ek < n; Ek += s << 1) {
                    int Ok = Ek + s;
                    ll t = w * a[Ok] % P;
                    a[Ok] = (a[Ek] - t) % P;
                    a[Ek] = (a[Ek] + t) % P;
                }
                w = w * wn % P;
            }
        }
        if (on == -1) {
            ll inv = fastpow(n, P - 2, P);
            for (int i = 0; i < n; i++)
                a[i] = a[i] * inv % P;
        }
    }

    std::vector<ll> operator*(const std::vector<ll> &v1, const std::vector<ll> &v2) {
        int S = 2, RES = v1.size() + v2.size() - 1;
        while (S <= RES)
            S <<= 1;
        std::vector<ll> a = v1;
        std::vector<ll> b = v2;
        a.resize(S), NTT(a, 1);
        b.resize(S), NTT(b, 1);
        for (int i = 0; i < S; i++)
            a[i] = a[i] * b[i] % P;
        NTT(a, -1), a.resize(RES);
        for (auto &x : a)
            x = (x + P) % P;
        return a;
    }

}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<NTT::ll> a(n + 1), b(m + 1);
    for (int i = 0; i <= n; i++) {
        std::cin >> a[i];
    }
    for (int i = 0; i <= m; i++) {
        std::cin >> b[i];
    }
    std::vector<NTT::ll> c = NTT::operator*(a, b);
    for (int i = 0; i < (int)c.size(); i++) {
        std::cout << c[i] << ' ';
    }
    return 0;
}