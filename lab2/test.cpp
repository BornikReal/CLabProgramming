#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> a = {9, 8, 7, 6, 5, 4};
    vector<int> b = {4, 3, 2, 1, 0};
    int pos1 = 0, pos2 = 0;
    bool d = false;
    while ((pos1 < a.size()) && (pos2 < b.size())) {
        if (a[pos1] > b[pos2]) {
            pos1++;
        }
        else if (a[pos1] < b[pos2]) {
            pos2++;
        }
        else {
            d = true;
            break;
        }
    }
    cout << d << endl;
    return 0;
}