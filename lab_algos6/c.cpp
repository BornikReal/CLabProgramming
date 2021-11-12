#include <iostream>
#include <vector>
#include <string>
using namespace std;

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