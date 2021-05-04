#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <const char BEG = '@', const char END = 0, const char INS = '.', typename String = std::string>
void manacher(const String &str, std::vector<int> &r) {
    int length = 2 * str.size() + 3;

    String s(length, 0);
    s[0] = BEG, s[1] = INS, s[length - 1] = END;
    for (int i = 0; i < (int)str.size(); i++) {
        s[2 * (i + 1) + 0] = str[i];
        s[2 * (i + 1) + 1] = s[1];
    }

    r = std::vector<int>(length, 0);
    r[1] = 1, r[2] = 2;

    int mid = 2, maxright = 3;
    for (int i = 3; i < length - 1; i++) {
        r[i] = maxright < i ? 1 : std::min(r[mid * 2 - i], maxright - i);
        while (s[i - r[i]] == s[i + r[i]])
            r[i]++;
        if (i + r[i] > maxright) {
            maxright = i + r[i];
            mid = i;
        }
    }
    for (auto &x : r) {
        x--;
    }
}

int main() {
    std::string str;
    while (std::cin >> str) {
        std::vector<int> r;
        Manacher(str, r);
        std::cout << *std::max_element(r.begin(), r.end()) << std::endl;
    }
    return 0;
}