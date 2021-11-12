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
    for (int i = 0; i < buf_com.size(); i++) {
        if (buf_com[i][0] == '+') {
            buf = qe.pop_back();
            buf += qe.pop_back();
            qe.push_back((buf + 65536) % 65536);
        }
        else if (buf_com[i][0] == '-') {
            buf = qe.pop_back();
            buf -= qe.pop_back();
            qe.push_back((buf + 65536) % 65536);
        }
        else if (buf_com[i][0] == '*') {
            buf = qe.pop_back();
            buf *= qe.pop_back();
            qe.push_back((buf + 65536) % 65536);
        }
        else if (buf_com[i][0] == '/') {
            buf = qe.pop_back();
            temp = qe.pop_back();
            if (temp == 0)
                buf = 0;
            else
                buf /= temp;
            qe.push_back((buf + 65536) % 65536);
        }
        else if (buf_com[i][0] == '%') {
            buf = qe.pop_back();
            temp = qe.pop_back();
            if (temp == 0)
                buf = 0;
            else
                buf %= temp;
            qe.push_back((buf + 65536) % 65536);
        }
        else if (buf_com[i][0] == '>')
            registers[buf_com[i][1] - 'a'] = qe.pop_back();
        else if (buf_com[i][0] == '<')
            qe.push_back(registers[buf_com[i][1] - 'a']);
        else if (buf_com[i] == "P")
            cout << qe.pop_back() << endl;
        else if (buf_com[i][0] == 'P')
            cout << registers[buf_com[i][1] - 'a'] << endl;
        else if (buf_com[i] == "C")
            cout << char(qe.pop_back());
        else if (buf_com[i][0] == 'C')
            cout << char(registers[buf_com[i][1] - 'a']);
        else if (buf_com[i][0] == ':')
            continue;
        else if (buf_com[i][0] == 'J') {
            input = buf_com[i].substr(1, buf_com.size() - 1);
            for (int j = 0; j < assoc.size(); j++) {
                if (assoc[j].first == input) {
                    i = assoc[j].second;
                    break;
                }
            }
        }
        else if (buf_com[i][0] == 'Z') {
            if (registers[buf_com[i][1] - 'a'] == 0) {
                input = buf_com[i].substr(2, buf_com.size() - 2);
                for (int j = 0; j < assoc.size(); j++) {
                    if (assoc[j].first == input) {
                        i = assoc[j].second;
                        break;
                    }
                }
            }
        }
        else if (buf_com[i][0] == 'E') {
            if (registers[buf_com[i][1] - 'a'] == registers[buf_com[i][2] - 'a']) {
                input = buf_com[i].substr(3, buf_com.size() - 3);
                for (int j = 0; j < assoc.size(); j++) {
                    if (assoc[j].first == input) {
                        i = assoc[j].second;
                        break;
                    }
                }
            }
        }
        else if (buf_com[i][0] == 'G') {
            if (registers[buf_com[i][1] - 'a'] > registers[buf_com[i][2] - 'a']) {
                input = buf_com[i].substr(3, buf_com.size() - 3);
                for (int j = 0; j < assoc.size(); j++) {
                    if (assoc[j].first == input) {
                        i = assoc[j].second;
                        break;
                    }
                }
            }
        }
        else if (buf_com[i][0] == 'Q')
            break;
        else
            qe.push_back(atoi(buf_com[i].c_str()));
    }
    return 0;
}