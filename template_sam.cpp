#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

struct SAM {
    static const int SIGMA = 26;
    static int encode(char c) {
        return c - 'a';
    }
    struct State {
        int len;
        int eps;
        State *link;
        std::array<State *, SAM::SIGMA> tr;
    } *start = nullptr, *last = nullptr;
    static const int MAXL = 1e6;
    static const int MAXP = 2 * MAXL - 1;
    State pool[MAXP];
    int pool_size = 0;
    void initial() {
        pool_size = 0;
        start = last = new (&pool[pool_size++]) State{0, 0, nullptr};
    }
    State *extend(char c) {
        auto val = encode(c);
        State *cur = new (&pool[pool_size++]) State{last->len + 1, 1, nullptr}, *p, *q;
        for (p = last; p && !(q = p->tr[val]); p = p->link) {
            p->tr[val] = cur;
        }
        if (!p) {
            cur->link = start;
        } else if (p->len + 1 == q->len) {
            cur->link = q;
        } else {
            auto clone = new (&pool[pool_size++]) State{p->len + 1, 0, q->link, q->tr};
            for (; p && p->tr[val] == q; p = p->link) {
                p->tr[val] = clone;
            }
            q->link = clone;
            cur->link = clone;
        }
        return last = cur;
    }
    void complete() {
        auto sort = std::vector<State *>(pool_size);
        for (int i = 0; i < pool_size; i++)
            sort[i] = pool + i;
        std::sort(sort.begin(), sort.end(), [](const State *lhs, const State *rhs) { return lhs->len > rhs->len; });
        for (auto u : sort) {
            if (u->link)
                u->link->eps += u->eps;
        }
    }
};

SAM sam;

int main() {
    std::ios::sync_with_stdio(false);
    std::string s;
    std::cin >> s;
    sam.initial();
    for (auto c : s) {
        sam.extend(c);
    }
    sam.complete();
    int ans = 0;
    std::for_each(sam.pool + 0, sam.pool + sam.pool_size, [&ans](SAM::State &s) {
        if (s.eps > 1)
            ans = std::max(ans, s.eps * s.len);
    });
    std::cout << ans << std::endl;
    return 0;
}
