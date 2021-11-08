#include <algorithm>
#include <cmath>
#include <cstdio>
#include <queue>
#include <stack>
#include <set>
#include <vector>

const double EPS = 1e-8;

int dcmp(double x) {
    if (std::abs(x) < EPS)
        return 0;
    return x < 0 ? -1 : 1;
}

struct Vector {
    double x, y;
    Vector operator+(const Vector rhs) const { return Vector{x + rhs.x, y + rhs.y}; }
    Vector operator-(const Vector rhs) const { return Vector{x - rhs.x, y - rhs.y}; }
    Vector operator*(const double rhs) const { return Vector{x * rhs, y * rhs}; }
    double abs() const { return std::sqrt(x * x + y * y); }
    double angle() const { return std::atan2(y, x); }
    double dot(Vector rhs) const { return x * rhs.x + y * rhs.y; }
    double cross(Vector rhs) const { return x * rhs.y - y * rhs.x; }
    bool operator<(const Vector rhs) const { return dcmp(x - rhs.x) != 0 ? dcmp(x - rhs.x) < 0 : dcmp(y - rhs.y) < 0; }
};

typedef Vector Point;
typedef std::pair<Point, Vector> Line;
typedef std::pair<Point, Point> Segment;
typedef std::vector<Vector> Polygon;

int onLine(const Point &p, const Line &l) {
    // -1 => point is on the right
    // 0 => point is on the line
    // +1 => point is on the left
    return dcmp(l.second.cross(p - l.first));
}

bool onSegment(const Point &p, const Segment &s) {
    // false => point is off the line
    // true => point is on the line
    return (s.first - p).cross(s.second - p) == 0 &&
           std::min(s.first.x, s.second.x) <= p.x && std::min(s.first.y, s.second.y) <= p.y &&
           std::max(s.first.x, s.second.x) >= p.x && std::max(s.first.y, s.second.y) >= p.y;
}

bool collinear(const Segment &a, const Segment &b) {
    return dcmp((a.second - a.first).cross(b.second - b.first)) == 0 &&
           dcmp((a.second - a.first).cross(b.second - a.first)) == 0 &&
           dcmp((b.second - b.first).cross(a.second - b.first)) == 0;
}

Point intersection(const Line &a, const Line &b) {
    double x = b.second.cross(a.first - b.first) / a.second.cross(b.second);
    return a.first + a.second * x;
}

bool isIntersected(const Segment &a, const Segment &b) {
    return std::max(a.first.x, a.second.x) >= std::min(b.first.x, b.second.x) &&
           std::max(a.first.y, a.second.y) >= std::min(b.first.y, b.second.y) &&
           std::max(b.first.x, b.second.x) >= std::min(a.first.x, a.second.x) &&
           std::max(b.first.y, b.second.y) >= std::min(a.first.y, a.second.y) &&
           dcmp((b.second - b.first).cross(a.first - b.first) * (b.second - b.first).cross(a.second - b.first)) == -1 &&
           dcmp((a.second - a.first).cross(b.first - a.first) * (a.second - a.first).cross(b.second - a.first)) == -1;
}

double area(const Polygon &polygon) {
    int n = polygon.size();
    double area2 = 0;
    for (int i = 0; i < n; i++) {
        double v = polygon[i].cross(polygon[(i + 1) % n]);
        area2 += v;
    }
    return area2 / 2;
}

Point gravity(const Polygon &polygon) {
    int n = polygon.size();
    Point ans{0, 0};
    for (int i = 0; i < n; i++) {
        double v = polygon[i].cross(polygon[(i + 1) % n]);
        ans = ans + (polygon[i] + polygon[(i + 1) % n]) * v;
    }
    return ans * (1 / 6 / area(polygon));
}

void androw(std::vector<Point> &s) {
    int n = s.size();
    std::sort(s.begin(), s.end());
    std::vector<Vector> ans;
    for (int i = 0; i < n; i++) {
        while (ans.size() >= 2 && dcmp((ans.back() - ans[ans.size() - 2]).cross(s[i] - ans.back())) < 0)
            ans.pop_back();
        ans.push_back(s[i]);
    }
    int height = ans.size();
    for (int i = n - 2; i >= 1; i--) {
        while (ans.size() - height >= 1 && dcmp((ans.back() - ans[ans.size() - 2]).cross(s[i] - ans.back())) < 0)
            ans.pop_back();
        ans.push_back(s[i]);
    }
    s = std::move(ans);
}

std::optional<Polygon> sai(std::vector<Line> &e) {
    std::sort(e.begin(), e.end(), [](const Line &x, const Line &y) {
        double xa = x.second.angle(), ya = y.second.angle();
        return dcmp(xa - ya) ? xa < ya : onLine(x.first, y) == 1;
    });
    std::deque<std::pair<Line, Point>> q;
    q.push_back(std::make_pair(e[0], Point{}));
    for (int i = 1; i < (int)e.size(); i++) {
        if (!dcmp(e[i].second.angle() - e[i - 1].second.angle()))
            continue;
        while (q.size() > 1 && onLine(q.back().second, e[i]) == -1)
            q.pop_back();
        while (q.size() > 1 && onLine(q[1].second, e[i]) == -1)
            q.pop_front();
        q.push_back(std::make_pair(e[i], intersection(e[i], q.back().first)));
    }
    while (q.size() > 1 && onLine(q.back().second, q.front().first) == -1)
        q.pop_back();
    q.front().second = intersection(q.front().first, q.back().first);
    if (q.size() <= 2) {
        return std::nullopt;
    } else {
        Polygon polygon;
        for (const auto &[_, v] : q)
            polygon.push_back(v);
        return polygon;
    }
}
