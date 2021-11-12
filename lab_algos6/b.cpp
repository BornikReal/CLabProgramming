#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen("check.in", "r", stdin);
    freopen("check.out", "w", stdout);
    int n, k, l, r;
    cin >> n;
    vector<vector<int> > array(n);
    for (int i = 0; i < n; i++) {
        cin >> k >> l >> r;
        array[i].push_back(k);
        array[i].push_back(l);
        array[i].push_back(r);
    }
    return 0;
}