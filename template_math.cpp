#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <optional>
#include <vector>

template <const int N>
struct Gauss {
    using Vector = std::array<double, N>;
    static constexpr double EPS = 1e-9;

    std::array<Vector, N> A;
    Vector B;
    Vector X;

    bool gauss() {
        std::array<int, N> s;
        std::fill(s.begin(), s.end(), -1);
        for (int i = 0; i < N; i++) {
            int f = -1;
            for (int j = 0; j < N; j++)
                if (s[j] == -1 && ((f == -1) || std::fabs(A[f][i]) < std::fabs(A[j][i])))
                    f = j;
            if (f == -1 || std::fabs(A[f][i]) < EPS)
                return false;
            s[f] = i;
            for (int j = 0; j < N; j++)
                if (j != f && A[j][i] != 0) {
                    double x = -A[j][i] / A[f][i];
                    for (int k = i; k < N; k++)
                        A[j][k] = A[j][k] + x * A[f][k];
                    B[j] = B[j] + x * B[f];
                }
        }
        for (int i = 0; i < N; i++)
            X[s[i]] = B[i] / A[i][s[i]];
        return true;
    }
};

template <typename T = uint64_t>
struct Basis {
    static constexpr int BITS = sizeof(T) * 8;

    std::array<T, BITS> p = {};
    int s = 0;
    bool z = false;

    void insert(T x) {
        for (int i = 0; i < BITS; i++) {
            if ((x >> i) & 1) {
                if (p[i]) {
                    x ^= p[i];
                }
            }
        }
        if (x) {
            int j = std::bit_width(x) - 1;
            for (int k = j + 1; k < BITS; k++) {
                if ((p[k] >> j) & 1) {
                    p[k] ^= x;
                }
            }
            p[j] = x, s++;
        } else {
            z = true;
        }
    }

    std::optional<T> query(T k) {
        if (z) {
            k--;
        }
        T ans = 0;
        if (k >> s) {
            return std::nullopt;
        }
        for (int i = 0; i < BITS; i++) {
            if (p[i]) {
                if (k & 1) {
                    ans ^= p[i];
                }
                k >>= 1;
            }
        }
        return ans;
    }
};

int main() {
    return 0;
}
