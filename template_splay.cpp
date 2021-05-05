#include <algorithm>
#include <iostream>

namespace Splay {
    struct Node {
        int data, cnt = 0;
        Node *fa = nullptr, *ch[2] = {};

        int size;
        void pushup() {
            size = cnt + (ch[0] ? ch[0]->size : 0) + (ch[1] ? ch[1]->size : 0);
        }

        Node(int _data) {
            data = _data;
            pushup();
        }

        int type() {
            return fa ? (fa->ch[0] == this ? 0 : 1) : -1;
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

            int d = type();
            adopt(fa->fa, this, fa->type());
            adopt(fa, ch[d ^ 1], d);
            adopt(this, fa, d ^ 1);
            fa->pushup();
            pushup();
        }
        void splay(const Node *goal = 0) {
            while (fa != goal) {
                if (fa->fa != goal)
                    (type() == fa->type() ? fa : this)->rotate();

                rotate();
            }
        }
    };

    struct Splay {
        void splay(Node *x) {
            if (x) {
                x->splay();
                root = x;
            }
        }
        Node *root = nullptr;
        std::pair<bool, Node *> search(int data) {
            Node *last = nullptr, *x = root;

            while (x) {
                if (data == x->data)
                    return std::make_pair(true, x);

                x = (last = x)->ch[data < x->data ? 0 : 1];
            }

            return std::make_pair(false, last);
        }
        Node *insert(int data) {
            auto f = search(data);

            if (f.first) {
                splay(f.second);
            }

            if (!f.first && root) {
                Node *x = new Node(data);
                Node::adopt(f.second, x, data < f.second->data ? 0 : 1);
                splay(x);
            }

            if (!f.first && !root) {
                root = new Node(data);
            }

            root->cnt++;
            root->pushup();
            return root;
        }
        bool find(int data) {
            auto f = search(data);
            splay(f.second);
            return f.first;
        }
        Node *findkth(int kth) {
            if (!(1 <= kth && kth <= root->size)) {
                return nullptr;
            }

            Node *x = root;
            while (true) {
                int left = x->ch[0] ? x->ch[0]->size : 0;

                if (kth <= left) {
                    x = x->ch[0];
                    continue;
                }

                kth -= left;
                int mid = x->cnt;

                if (kth <= mid) {
                    splay(x);
                    return x;
                }

                kth -= mid;
                x = x->ch[1];
            }
        }
        int countleft(int data) {
            if (!root)
                return 0;

            find(data);
            int ans = 0;
            ans += root->ch[0] ? root->ch[0]->size : 0;
            ans += data > root->data ? root->cnt : 0;
            return ans;
        }
        Node *findextre(Node *x, int d) {
            if (!x)
                return 0;

            while (x->ch[d])
                x = x->ch[d];

            return x;
        }
        bool remove(int data) {
            if (!find(data))
                return false;

            root->cnt--;
            root->pushup();

            if (root->cnt > 0)
                return true;

            Node *x = root;

            if (!root->ch[0] && !root->ch[1]) {
                delete x;
                root = 0;
                return true;
            }

            if (!root->ch[0] || !root->ch[1]) {
                int d = root->ch[1] ? 1 : 0;
                root = root->ch[d];
                root->fa = NULL;
            } else {
                Node *y = root->ch[0];
                y->fa = 0;
                splay(findextre(y, 1));
                Node::adopt(root, x->ch[1], 1);
            }

            root->pushup();
            delete x;
            return true;
        }
    };
}; // namespace Splay

int main() {
    std::ios::sync_with_stdio(false);
    int n, opt, x;
    Splay::Splay S;
    std::cin >> n;

    while (n--) {
        std::cin >> opt >> x;

        switch (opt) {
        case 1:
            S.insert(x);
            break;

        case 2:
            S.remove(x);
            break;

        case 3:
            std::cout << (S.countleft(x) + 1) << std::endl;
            break;

        case 4:
            std::cout << S.findkth(x)->data << std::endl;
            break;

        case 5:
            S.find(x);

            if (S.root->data < x)
                std::cout << S.root->data << std::endl;
            else {
                Splay::Node *ans = S.findextre(S.root->ch[0], 1);
                std::cout << ans->data << std::endl;
            }

            break;

        case 6:
            S.find(x);

            if (S.root->data > x)
                std::cout << S.root->data << std::endl;
            else {
                Splay::Node *ans = S.findextre(S.root->ch[1], 0);
                std::cout << ans->data << std::endl;
            }

            break;
        }
    }

    return 0;
}