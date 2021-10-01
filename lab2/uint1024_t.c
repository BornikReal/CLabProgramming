#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

struct uint1024_t
{
    uint8_t num[155];
    uint8_t last_pos;
};

uint8_t pos(int x, int y) {
    if (x > y)
        return x;
    else
        return y;
}

struct uint1024_t from_uint(unsigned int x) {
    struct uint1024_t ret;
    int pos = 0;
    while (x > 0) {
        ret.num[pos] = x % 100;
        pos++;
        x /= 100;
    }
    ret.last_pos = pos;
    for (int i = pos; i < 155; i++)
        ret.num[i] = 0;
    return ret;
}

struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y) {
    int buffer = 0;
    struct uint1024_t result;
    result.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < result.last_pos; i++) {
        result.num[i] = (x.num[i] + y.num[i] + buffer) % 100;
        buffer = (x.num[i] + y.num[i] + buffer) / 100;
    }
    return result;
}

struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y) {
    int buffer = 0;
    struct uint1024_t result;
    result.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < result.last_pos; i++) {
        result.num[i] = (100 + x.num[i] - y.num[i] - buffer) % 100;
        buffer = ((x.num[i] - y.num[i] - buffer) < 0 ? 1 : 0);
    }
    return result;
}

void printf_value(struct uint1024_t x) {
    for (int i = (x.last_pos - 1); i >= 0; i--)
        printf("%d", x.num[i]);
}

void scanf_value(struct uint1024_t* x) {
    char temp[310];
    // x -> last_pos = 0;
    fgets(temp, 310, stdin);
    for (int i = 0; i < 310; i++) {
        int digit = temp[i] - '0';
        if ((digit < 0) || (digit > 9))
            temp[i] = '0';
    }
    for (int i = 0; i < 309; i += 2) {
        // printf("%c %c\n", temp[i], temp[i - 1]);
        int first_dig = temp[i] - '0';
        int second_dig = temp[i + 1] - '0';
        printf("%d %d\n", first_dig, second_dig);
        if ((first_dig >= 0) && (first_dig <= 9) && (second_dig >= 0) && (second_dig <= 9)) {
            if ((first_dig == 0) && (second_dig == 0))
                x -> last_pos = (i / 2) + 1;
            x -> num[i / 2] = first_dig * 10 + second_dig;
        }
    }
    if (x -> num[154] != 0)
        x -> last_pos = 155;
}

int main() {
    struct uint1024_t t1, t;
    // scanf_value(&t);
    t1 = from_uint(100000);
    t = from_uint(145);
    t = subtr_op(t1, t);
    printf_value(t);
    // char s[309];
    // itoa(2131241241234234243534, s, 10);
    // printf("%s", s);
    return 0;
}