#include <algorithm>
#include <array>
#include <cmath>

template <const int N>
struct Gauss {
    using Vector = std::array<double, N>;
    static constexpr double eps = 1e-9;

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
            if (f == -1 || std::fabs(A[f][i]) < eps)
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

int main() {
    return 0;
}
