#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct uint1024_t
{
    uint8_t num[129];
    uint8_t last_pos;
} uint1024_t;

uint8_t pos(uint8_t x, uint8_t y) {
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
    if (x == 0) {
        result.last_pos = 1;
        for (int i = 0; i < 129; i++)
            result.num[i] = 0;
            return result;
    }
    uint8_t pos = 0;
    while (x > 0)
    {
        result.num[pos] = x % 256;
        x /= 256;
        pos++;
    }
    result.last_pos = pos;
    for (int i = pos; i < 129; i++)
        result.num[i] = 0;
    return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint8_t buffer = 0;
    x.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < x.last_pos; i++) {
        uint8_t temp = buffer;
        buffer = (x.num[i] + y.num[i] + buffer) / 256;
        x.num[i] = (x.num[i] + y.num[i] + temp) % 256;
    }
    if (x.num[x.last_pos - 1] == 0)
        x.last_pos--;
    for (int i = x.last_pos; i < 129; i++)
        x.num[i] = 0;
    return x;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint8_t buffer = 0;
    x.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < x.last_pos; i++) {
        uint8_t temp = buffer;
        buffer = ((x.num[i] - y.num[i] - buffer) < 0 ? 1 : 0);
        x.num[i] = (256 + x.num[i] - y.num[i] - temp) % 256;
    }
    for (int i = (x.last_pos - 1); i > 0; i--) {
        if (x.num[i] == 0)
            x.last_pos--;
        else
            break;
    }
    for (int i = x.last_pos; i < 129; i++)
        x.num[i] = 0;
    return x;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    int buffer;
    uint1024_t result = from_uint(0), temp;
    for (int i = 0; i < x.last_pos; i++) {
        temp = from_uint(0);
        buffer = 0;
        for (uint8_t j = 0; j < y.last_pos; j++) {
            if ((i + j) < 129)
                temp.num[i + j] = (x.num[i] * y.num[j] + buffer) % 256;
            buffer = (x.num[i] * y.num[j] + buffer) / 256;
        }
        temp.last_pos = y.last_pos + i;
        if ((buffer != 0) && (temp.last_pos != 129)) {
            temp.num[temp.last_pos] += (buffer % 256);
            temp.last_pos++;
        }
        for (uint8_t j = (temp.last_pos - 1); j >= 1; j--) {
            if (temp.num[j] == 0)
                temp.last_pos--;
            else
                break;
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

int compare_op(uint1024_t x, uint1024_t y) {
    if (x.last_pos > y.last_pos)
        return 1;
    else if (x.last_pos < y.last_pos)
        return -1;
    else {
        for (int i = (x.last_pos - 1); i >= 0; i--) {
            if (x.num[i] > y.num[i])
                return 1;
            else if (x.num[i] < y.num[i])
                return -1;
        }
        return 0;
    }
}

uint1024_t getpart_op(uint1024_t x, uint8_t len) {
    uint1024_t result = from_uint(0);
    if ((len == 0) || (x.last_pos < len))
        return result;
    result.last_pos = len;
    for (int i = (len - 1); i >= 0; i--)
        result.num[i] = x.num[x.last_pos - (len - i)];
    return result;
}

uint8_t subtr_first_op(uint1024_t *x, uint1024_t y) {
    uint8_t buffer = 0, additional = 0, num = 0;
    if (compare_op(*x, y) != 1)
        return 0;
    if (compare_op(getpart_op(*x, y.last_pos), y) == -1)
        additional++;
    uint1024_t temp = getpart_op(*x, y.last_pos + additional), temp1;
    unsigned t = 1;
    for (int i = 1; i <= (x -> last_pos - y.last_pos - additional); i++)
        t *= 256;
    if (compare_op(*x, mult_op(temp, from_uint(t))) != -1)
        temp1 = subtr_op(*x, mult_op(temp, from_uint(t)));
    else
        temp1 = from_uint(0);
    while (compare_op(temp, y) == 1)
    {
        temp = subtr_op(temp, y);
        num++;
    }
    *x = add_op(mult_op(temp, from_uint(t)), temp1);
    return num;
}

uint1024_t del_op(uint1024_t x, uint1024_t y) {
    uint1024_t result = from_uint(0);
    uint8_t t = 1;
    while (t != 0) {
        t = subtr_first_op(&x, y);
        if (t > 0)
            result = add_op(mult_op(result, from_uint(256)), from_uint(t));
    }
    return result;
}

uint1024_t mod_op(uint1024_t x, uint1024_t y) {
    uint1024_t result = from_uint(0);
    uint8_t t = 1;
    while (t != 0) {
        t = subtr_first_op(&x, y);
        if (t > 0)
            result = add_op(mult_op(result, from_uint(256)), from_uint(t));
    }
    return x;
}

void printf_value(uint1024_t x) {
    uint8_t output[309], pos = 0;
    uint1024_t zero = from_uint(0), ten = from_uint(10);
    while (compare_op(x, zero) == 1) {
        output[pos] = mod_op(x, ten).num[0];
        x = del_op(x, ten);
        pos++;
    }
    for (int i = (pos - 1); i >= 0; i--)
        printf("%d", output[i]);
    printf("\n");
}

int main() {
    // uint1024_t t = from_uint(213), t1 = from_uint(12312);
    // printf_value(del_op(t, t1));
    // printf_value(mod_op(t, t1));
    // uint1024_t t = from_uint(0);
    // t = mult_op(t, from_uint(10));
    // scanf_value(&t);
    // printf_value(t);
    uint1024_t t = from_uint(12342412), t1 = from_uint(23424), ten = from_uint(10);
    printf_value(t);
    printf_value(t1);
    // t = del_op(t, t1);
    // t = mod_op(t1, ten);
    // printf_value(t);
    // printf("%d", subtr_first_op(&t, t1));
    // for (int i = 1; i < 1024; i++) {
    //     t = mult_op(t, t1);
    // }
    // printf_value(t);
    // t = mult_op(t, t1);
    // uint1024_t t = from_uint(15000), t1 = from_uint(21), sum, sub, mult;
    // printf_value(t);
    // printf_value(t1);
    // t = subtr_first_op(t, t1);
    // printf_value(t);
    // printf_value(getpart_op(t, 4));
    // sum = add_op(t1, t);
    // sub = subtr_op(t, t1);
    // mult = mult_op(t, t1);
    // mult = mult_op(mult, t1);
    // printf_value(mult);
    // mult = mult_op(mult, t1);
    // printf_value(mult);
    // mult = mult_op(mult, t1);
    // printf_value(sum);
    // printf_value(sub);
    // printf_value(mult);
    return 0;
}