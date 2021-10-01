#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

struct temp1024
{
    uint8_t num[309];
    uint8_t last_pos;
};

struct temp1024 long_multiply(uint8_t x, uint8_t y) {
    struct temp1024 output_num;
    output_num.num[0] = x;
    for (int i = 1; i < 309; i++)
        output_num.num[i] = 0;
    // for (int i = 0; i < 127; i++) {
    //     int buffer = 0;
    //     do {

    //     } while ((buffer != 0) || ());
    // }
}

struct uint1024_t
{
    uint8_t num[128];
    uint8_t last_pos;
};

uint8_t pos(int x, int y) {
    if (x > y)
        return x;
    else
        return y;
}

struct uint1024_t from_uint(unsigned int x) {
    struct uint1024_t result;
    uint8_t pos = 0;
    while (x > 0)
    {
        result.num[pos] = x % 256;
        x /= 256;
    }
    result.last_pos = pos;
    for (uint8_t i = pos; i < 128; i++)
        result.num[i] = 0;
    return result;
}

struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y) {
    int buffer = 0;
    struct uint1024_t result;
    result.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < result.last_pos; i++) {
        result.num[i] = (x.num[i] + y.num[i] + buffer) % 256;
        buffer = (x.num[i] + y.num[i] + buffer) / 256;
    }
    return result;
}

struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y) {
    int buffer = 0;
    struct uint1024_t result;
    result.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < result.last_pos; i++) {
        result.num[i] = (100 + x.num[i] - y.num[i] - buffer) % 256;
        buffer = ((x.num[i] - y.num[i] - buffer) < 0 ? 1 : 0);
    }
    return result;
}

void printf_value(struct uint1024_t x) {
    struct temp1024 output_num;
    for (int i = 0; i < 309; i++)
        output_num.num[i] = 0;
    // for (int i = 0; i = x.last_pos; i++)
        // output_num += long_multiply(x[i], i);
}