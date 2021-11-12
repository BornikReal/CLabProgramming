#include <iostream>
#include <vector>
#include <string>

using namespace std;

class queue {
	private:
        struct cell {
            uint16_t value;
            cell* next;
        };
		cell *Last = 0;
		cell *First = 0;
	public:
		void push_back(uint16_t x) {
			cell *new_cell = new cell({x, 0});
			if (First)
                First -> next = new_cell;
			First = new_cell;
			if (!Last)
                Last = new_cell;
		}
		uint16_t pop_back() {
			uint16_t deleted = Last -> value;
			cell *old_cell = Last;
			if (First == Last)
				First = 0;
			if (Last)
				Last = Last -> next;
			delete old_cell;
			return deleted;
		}
};

vector<string> buf_com;
vector<pair<string, int> > assoc;
vector<uint16_t> registers(26);

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen("quack.in", "r", stdin);
    freopen("quack.out", "w", stdout);
    string input;
    int buf, temp;
    queue qe;
    while(cin >> input) {
        buf_com.push_back(input);
        if (input[0] == ':')
            assoc.push_back(make_pair(input.substr(1, input.size() - 1), buf_com.size() - 1));
    }
    for (int i = 0; i < 26; i++)
        registers[i] = 0;
    return 0;
}