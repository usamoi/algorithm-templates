#include <algorithm>
#include <iostream>
#include <vector>

namespace Linca {
    struct Node {
        Node *fa = nullptr, *ch[2] = {}, *pathparent = nullptr;
        char rev = 0;
        int value, data = 0;

        Node() {}
        Node(int _value) {
            value = _value;
            pushup();
        }

        void pushup() {
            data = value;
            if (ch[0])
                data ^= ch[0]->data;
            if (ch[1])
                data ^= ch[1]->data;
        }
        void pushdown() {
            if (rev == 0)
                return;
            if (ch[0])
                ch[0]->rev ^= 1;
            if (ch[1])
                ch[1]->rev ^= 1;
            std::swap(ch[0], ch[1]);
            rev = 0;
        }

        int type() {
            if (fa)
                return fa->ch[0] == this ? 0 : 1;
            return -1;
        }
        static void adopt(Node *x, Node *y, int z) {
            if (x && (z != -1))
                x->ch[z] = y;
            if (y)
                y->fa = x;
        }
        void rotate() {
            Node *fa = this->fa;
            if (!fa)
                return;

            if (fa->fa)
                fa->fa->pushdown();
            fa->pushdown();
            pushdown();

            int d = type();
            adopt(fa->fa, this, fa->type());
            adopt(fa, ch[d ^ 1], d);
            adopt(this, fa, d ^ 1);

            fa->pushup();
            pushup();
        }
        void splay() {
            if (fa != 0) {
                Node *r = this;
                while (r->fa)
                    r = r->fa;
                std::swap(r->pathparent, pathparent);
            }
            while (fa != 0) {
                if (fa->fa != 0) {
                    fa->fa->pushdown();
                    fa->pushdown();
                    (type() == fa->type() ? fa : this)->rotate();
                }
                rotate();
            }
        }

        void expose() {
            splay();
            pushdown();
            if (ch[1]) {
                ch[1]->fa = 0;
                ch[1]->pathparent = this;
                ch[1] = 0;
                pushup();
            }
        }
        bool splice() {
            splay();
            if (!pathparent)
                return false;

            pathparent->expose();
            pathparent->ch[1] = this;
            fa = pathparent;
            pathparent = 0;
            fa->pushup();

            return true;
        }
        void access() {
            expose();
            while (splice())
                ;
        }
        Node *getroot() {
            access();
            splay();
            Node *v = this;
            v->pushdown();
            while (v->ch[0]) {
                v = v->ch[0];
                v->pushdown();
            }
            return v;
        }
        void evert() {
            access();
            splay();
            rev ^= 1;
        }
    };
    void update(Node *u, int val) {
        u->splay();
        u->value = val;
        u->pushup();
    }
    Node *query(Node *u, Node *v) { // promise u->v exists
        u->evert();
        v->access();
        v->splay();
        return v;
    }
    bool linked(Node *u, Node *v) {
        u->evert();
        v->access();
        v->splay();
        v->pushdown(), u->pushdown();
        return v->ch[0] == u && u->ch[1] == 0;
    }
    void link(Node *u, Node *v) {
        v->evert();
        v->pathparent = u;
    }
    void cut(Node *u, Node *v) {
        u->evert();
        v->access();
        u->expose();
        v->pathparent = 0;
    }
    typedef std::vector<Node> Linca;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    auto linca = Linca::Linca(1 + n);
    for (int i = 1, val; i <= n; i++) {
        std::cin >> val;
        linca[i] = Linca::Node(val);
    }
    while (m--) {
        int op, x, y;
        std::cin >> op >> x >> y;
        switch (op) {
        case 0:
            std::cout << Linca::query(&linca[x], &linca[y])->data << std::endl;
            break;
        case 1:
            if (!Linca::linked(&linca[x], &linca[y]))
                Linca::link(&linca[x], &linca[y]);
            break;
        case 2:
            if (Linca::linked(&linca[x], &linca[y]))
                Linca::cut(&linca[x], &linca[y]);
            break;
        case 3:
            Linca::update(&linca[x], y);
            break;
        }
    }
    return 0;
}