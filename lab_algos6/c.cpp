#include <iostream>
#include <vector>
#include <string>
using namespace std;

class node
{
private:
    struct leaf
    {
        int key;
        leaf *parent;
        leaf *left;
        leaf *right;
    };
    leaf *root = nullptr;
    int size = 0;
    leaf *find(int x, leaf *cur)
    {
        if (cur == nullptr)
            return nullptr;
        if (x > cur->key)
        {
            if (cur->right == nullptr)
                return cur;
            return find(x, cur->right);
        }
        else if (x < cur->key)
        {
            if (cur->left == nullptr)
                return cur;
            return find(x, cur->left);
        }
        else
            return cur;
    }
    leaf *next(int x, leaf *cur, leaf *minimum)
    {
        if (cur == nullptr)
            return minimum;
        if (cur->key <= x)
        {
            if (cur->right == nullptr)
                return minimum;
            return next(x, cur->right, minimum);
        }
        else
        {
            return next(x, cur->left, cur);
        }
    }
    leaf *prev(int x, leaf *cur, leaf *maximum)
    {
        if (cur == nullptr)
            return maximum;
        if (cur->key >= x)
        {
            if (cur->left == nullptr)
                return maximum;
            return prev(x, cur->left, maximum);
        }
        else
        {
            return prev(x, cur->right, cur);
        }
    }
    int delete_simple(leaf *cur)
    {
        if ((cur->left == nullptr) && (cur->right == nullptr))
        {
            if ((size == 1) || (cur == root))
                root = nullptr;
            if (cur->parent != nullptr)
            {
                if (cur->parent->left == cur)
                    cur->parent->left = nullptr;
                else
                    cur->parent->right = nullptr;
            }
            int deleted = cur->key;
            delete (cur);
            return deleted;
        }
        if (cur->left != nullptr)
        {
            if (root == cur)
            {
                root = cur->left;
                cur->left->parent = nullptr;
            }
            else
            {
                cur->left->parent = cur->parent;
                if (cur->parent->left == cur)
                    cur->parent->left = cur->left;
                else
                    cur->parent->right = cur->left;
            }
        }
        else
        {
            if (root == cur)
            {
                root = cur->right;
                cur->right->parent = nullptr;
            }
            else
            {
                cur->right->parent = cur->parent;
                if (cur->parent->left == cur)
                    cur->parent->left = cur->right;
                else
                    cur->parent->right = cur->right;
            }
        }
        int deleted = cur->key;
        delete (cur);
        return deleted;
    }
public:
    void insert(int x)
    {
        if (!exists(x))
        {
            size++;
            leaf *new_el = new leaf({x, nullptr, nullptr, nullptr});
            if (root == nullptr)
                root = new_el;
            else
            {
                leaf *comp = root;
                while (true)
                {
                    if (x > comp->key)
                    {
                        if (comp->right == nullptr)
                            break;
                        comp = comp->right;
                    }
                    else
                    {
                        if (comp->left == nullptr)
                            break;
                        comp = comp->left;
                    }
                }
                new_el->parent = comp;
                if (new_el->key > comp->key)
                    comp->right = new_el;
                else
                    comp->left = new_el;
            }
        }
    }
    bool exists(int x)
    {
        leaf *cur = find(x, root);
        if (cur == nullptr)
            return false;
        else if (cur->key != x)
            return false;
        else
            return true;
    }
    pair<bool, int> next(int x)
    {
        leaf *cur = next(x, root, nullptr);
        if (cur == nullptr)
            return make_pair(false, 0);
        if (cur->key <= x)
            return make_pair(false, 0);
        else
            return make_pair(true, cur->key);
    }
    pair<bool, int> prev(int x)
    {
        leaf *cur = prev(x, root, nullptr);
        if (cur == nullptr)
            return make_pair(false, 0);
        if (cur->key >= x)
            return make_pair(false, 0);
        else
            return make_pair(true, cur->key);
    }
    void delete_leaf(int x)
    {
        if (exists(x))
        {
            leaf *cur = find(x, root);
            if ((cur->left == nullptr) || (cur->right == nullptr))
                delete_simple(cur);
            else
            {
                leaf *next_el = next(cur->key, root, nullptr);
                cur->key = next_el->key;
                delete_simple(next_el);
            }
            size--;
        }
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen("bstsimple.in", "r", stdin);
    freopen("bstsimple.out", "w", stdout);
    string input;
    long long int first;
    bool ans;
    pair<bool, int> output;
    node bst;
    while (cin >> input)
    {
        if (input == "insert")
        {
            cin >> first;
            bst.insert(first);
        }

        else if (input == "delete")
        {
            cin >> first;
            bst.delete_leaf(first);
        }
        else if (input == "exists")
        {
            cin >> first;
            ans = bst.exists(first);
            if (ans)
                cout << "true" << endl;
            else
                cout << "false" << endl;
        }
        else if (input == "next")
        {
            cin >> first;
            output = bst.next(first);
            if (output.first)
                cout << output.second << endl;
            else
                cout << "none" << endl;
        }
        else if (input == "prev")
        {
            cin >> first;
            output = bst.prev(first);
            if (output.first)
                cout << output.second << endl;
            else
                cout << "none" << endl;
        }
    }
    return 0;
}