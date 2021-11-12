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
            // bst.insert(first);
        }

        else if (input == "delete")
        {
            cin >> first;
            // bst.delete_leaf(first);
        }
        else if (input == "exists")
        {
            cin >> first;
            // ans = bst.exists(first);
            if (ans)
                cout << "true" << endl;
            else
                cout << "false" << endl;
        }
        else if (input == "next")
        {
            cin >> first;
            // output = bst.next(first);
            if (output.first)
                cout << output.second << endl;
            else
                cout << "none" << endl;
        }
        else if (input == "prev")
        {
            cin >> first;
            // output = bst.prev(first);
            if (output.first)
                cout << output.second << endl;
            else
                cout << "none" << endl;
        }
    }
    return 0;
}