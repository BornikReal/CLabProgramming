#include <iostream>
#include <vector>
using namespace std;

int previ = INT_MIN;
bool isright = true;

bool check_array(vector<vector<int> > &array, int cur = 0) {
    if (array[cur][1] != 0)
        check_array(array, array[cur][1] - 1);
    if (array[cur][0] <= previ)
        isright = false;
    previ = array[cur][0];
    if (!isright)
        return false;
    if (array[cur][2] != 0)
        check_array(array, array[cur][2] - 1);
    return isright;
}

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
    if (array.size() == 0) {
        cout << "YES" << endl;
        return 0;
    }
    if (check_array(array))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    return 0;
}