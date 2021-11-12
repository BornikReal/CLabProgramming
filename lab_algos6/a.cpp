#include <iostream>
#include <vector>
using namespace std;

int height(vector<vector<int> > &array, int h = 1, int cur = 0) {
    if (array.size() == 0)
        return 0;
    int max = -1, t;
    if (array[cur][1] != 0)
        max = height(array, h + 1, array[cur][1] - 1);
    if (array[cur][2] != 0) {
        t = height(array, h + 1, array[cur][2] - 1);
        if (t > max)
             max = t;
    }
    if (max != -1)
        return max;
    else
        return h;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	freopen("height.in", "r", stdin);
	freopen("height.out", "w", stdout);
    int n, k, l, r;
    cin >> n;
    vector<vector<int> > array(n);
    for (int i = 0; i < n; i++) {
        cin >> k >> l >> r;
        array[i].push_back(k);
        array[i].push_back(l);
        array[i].push_back(r);
    }
    cout << height(array) << endl;
    return 0;
}