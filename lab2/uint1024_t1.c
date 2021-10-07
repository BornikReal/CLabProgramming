#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct uint1024_t
{
    uint8_t num[129];
    uint8_t last_pos;
} uint1024_t;

uint8_t pos(int x, int y) {
    if (x > y) {
        if ((x + 1) >= 129)
            return 129;
        else;
         return (x + 1);
    }
    else {
        if ((y + 1) >= 129)
            return 129;
        else;
         return (y + 1);
    }
}

uint1024_t from_uint(unsigned int x) {
    uint1024_t result;
    uint8_t pos = 0;
    while (x > 0)
    {
        result.num[pos] = x % 256;
        x /= 256;
        pos++;
    }
    result.last_pos = pos;
    for (uint8_t i = pos; i < 129; i++)
        result.num[i] = 0;
    if (x == 0)
        result.last_pos = 1;
    return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    int buffer = 0;
    uint1024_t result = from_uint(0);
    result.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < result.last_pos; i++) {
        result.num[i] = (x.num[i] + y.num[i] + buffer) % 256;
        buffer = (x.num[i] + y.num[i] + buffer) / 256;
    }
    if (result.num[result.last_pos - 1] == 0)
        result.last_pos--;
    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    int buffer = 0;
    uint1024_t result = from_uint(0);
    result.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < result.last_pos; i++) {
        result.num[i] = (256 + x.num[i] - y.num[i] - buffer) % 256;
        buffer = ((x.num[i] - y.num[i] - buffer) < 0 ? 1 : 0);
    }
    if (result.num[result.last_pos - 1] == 0)
        result.last_pos--;
    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint8_t buffer;
    uint1024_t result = from_uint(0), temp;
    for (int i = 0; i < x.last_pos; i++) {
        temp = from_uint(0);
        buffer = 0;
        for (int j = 0; j < y.last_pos; j++) {
            temp.num[i] = (x.num[i] * y.num[j] + buffer) % 256;
            buffer = (x.num[i] * y.num[j] + buffer) / 256;
        }
        temp.last_pos = y.last_pos + i;
        if (buffer != 0) {
            temp.num[temp.last_pos] += (buffer % 256);
            temp.last_pos++;
        }
        result = add_op(result, temp);
    }
    if ((result.num[result.last_pos - 1] == 0) && (result.last_pos != 1))
        result.last_pos--;
    return result;
}

void scanf_value(uint1024_t* x) {
    *x = from_uint(0);
    char input[309];
    scanf("%s", input);
    int i = 0;
    while (i < 309) {
        if (input[i] != '\0')
            *x = add_op(mult_op(*x, from_uint(10)), from_uint(input[i] - '0'));
        else
            break;
        i++;
    }
}

// void longadd(uint8_t *x) {
//     int buffer = 0;
// }

void printf_value(uint1024_t x) {
    // printf("%d\n", x.last_pos);
    // for (int i = (x.last_pos - 1); i >= 0; i--) {
    //     printf("%d ", x.num[i]);
    // }
//     // for (int i = 128; i >= 0; i--) {
//     //     printf("%d ", x.num[i]);
//     // }
//     uint8_t input[309];
//     int buffer = 0;
//     for (int i = 0; i < 309; i++)
//         input[i] = 0;
//     // for (int i = 0; i < 309; i++) {
//         // result.num[i] = (x.num[i] + y.num[i] + buffer) % 256;
//         // buffer = (x.num[i] + y.num[i] + buffer) / 256;
//     // }
    // printf("\n");
}


int main() {
    // uint1024_t t = from_uint(0);
    // t = mult_op(t, from_uint(10));
    // scanf_value(&t);
    // printf_value(t);
    // uint1024_t t = from_uint(256), t1 = from_uint(2);
    // for (int i = 1; i < 1024; i++) {
    //     t = mult_op(t, t1);
    // }
    // printf_value(t);
    // t = mult_op(t, t1);
    // printf_value(t);
    // uint1024_t t = from_uint(15000), t1 = from_uint(21), sum, sub, mult;
    // sum = add_op(t1, t);
    // sub = subtr_op(t1, t);
    // mult = mult_op(t1, t);
    // printf_value(sum);
    // printf_value(sub);
    // printf_value(mult);
    return 0;
}