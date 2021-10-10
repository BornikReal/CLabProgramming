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

void shift(uint1024_t *x, int pos) {
    if (pos < 0) {
        pos = -pos;
        if (x->last_pos <= pos)
            *x = from_uint(0);
        else {
            x->last_pos = x->last_pos - pos;
            for (int i = 0; i < x->last_pos; i++)
                x->num[i] = x->num[i + pos];
            for (int i = x->last_pos; i <= 128; i++)
                x->num[i] = 0;
            for (int i = x->last_pos - 1; i > 0; i--) {
                if (x->num[i] == 0)
                    x->last_pos--;
                else
                    break;
            }
        }
    }
    else {
        if ((x->last_pos + pos - 1) < 129)
            x->last_pos = x->last_pos + pos;
        for (int i = (x->last_pos - 1); i >= pos; i--)
            x->num[i] = x->num[i - pos];
        for (int i = (pos - 1); i >= 0; i--)
            x->num[i] = 0;
        for (int i = x->last_pos - 1; i > 0; i--) {
            if (x->num[i] == 0)
                x->last_pos--;
            else
                break;
        }
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

uint1024_t small_del_op(uint1024_t x, uint1024_t y) {
    uint1024_t result = from_uint(0), one = from_uint(1);
    while (compare_op(x, y) != -1)
    {
        x = subtr_op(x, y);
        result = add_op(result, one);
    }
    return result;
}

uint1024_t small_mod_op(uint1024_t x, uint1024_t y) {
    uint1024_t result = from_uint(0), one = from_uint(1);
    while (compare_op(x, y) != -1)
    {
        x = subtr_op(x, y);
        result = add_op(result, one);
    }
    return x;
}

uint1024_t getlastpart_op(uint1024_t x, int len) {
    uint1024_t result = from_uint(0);
    result.last_pos = len;
    for (int i = (len - 1); i >= 0; i--)
        result.num[i] = x.num[x.last_pos - (len - i)];
    return result;
}

uint1024_t getfirstpart_op(uint1024_t x, int len) {
    uint1024_t result = from_uint(0);
    if ((len == 0) || (x.last_pos < len))
        return result;
    result.last_pos = len;
    for (int i = (len - 1); i >= 0; i--)
        result.num[i] = x.num[i];
    return result;
}

uint1024_t merge(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    result.last_pos = x.last_pos + y.last_pos;
    int pos = result.last_pos - 1;
    for (int i = (x.last_pos - 1); i >= 0; i--) {
        result.num[pos] = x.num[i];
        pos--;
    }
    for (int i = (y.last_pos - 1); i >= 0; i--) {
        result.num[pos] = y.num[i];
        pos--;
    }
    return result;
}

// uint1024_t betamult_op(uint1024_t x, uint1024_t y) {
//     uint1024_t result = from_uint(0);
//     if (x.last_pos == 1) {
//         result.num[0] = (x.num[0] * y.num[0]) % 256;
//         result.num[1] = (x.num[0] * y.num[0]) / 256;
//         if (result.num[1] == 0)
//             result.last_pos = 1;
//         else
//             result.last_pos = 2;
//         return result;
//     }
//     int max_len = x.last_pos;
//     if (y.last_pos > max_len)
//         max_len = y.last_pos;
//     if (max_len % 2 != 0)
//         max_len++;
//     uint1024_t x1 = getfirstpart_op(x, max_len / 2), x2 = getlastpart_op(x, max_len / 2);
//     uint1024_t y1 = getfirstpart_op(y, max_len / 2), y2 = getlastpart_op(y, max_len / 2);
//     uint1024_t sum1 = add_op(x1, x2), sum2 = add_op(y1, y2);
//     uint1024_t first = betamult_op(x1, y1), second = betamult_op(x2, y2), third = betamult_op(sum1, sum2);
//     return result;
// }

uint1024_t del_op(uint1024_t x, uint1024_t y) {
    uint1024_t current, other, result = from_uint(0), zero = from_uint(0);
    int len;
    bool d;
    current = getlastpart_op(x, y.last_pos);
    other = getfirstpart_op(x, (x.last_pos - y.last_pos));
    len = x.last_pos - y.last_pos;
    if (compare_op(current, y) == -1) {
        current = getlastpart_op(x, y.last_pos + 1);
        other = getfirstpart_op(x, (x.last_pos - (y.last_pos + 1)));
        len = x.last_pos - (y.last_pos + 1);
    }
    while (len >= 0) {
        shift(&result, 1);
        for (int i = current.last_pos - 1; i > 0; i--) {
            if (current.num[i] == 0)
                current.last_pos--;
            else
                break;
        }
        result = add_op(result, small_del_op(current, y));
        if (compare_op(current, y) != -1)
            current = small_mod_op(current, y);
        current = merge(current, getlastpart_op(other, 1));
        d = true;
        for (int i = other.last_pos - 1; i >= 0; i--) {
            if (other.num[i] != 0) {
                d = false;
                break;
            }
        }
        if ((compare_op(current, zero) == 0) && d) 
            break;
        other = getfirstpart_op(other, (other.last_pos - 1));
        len--;
    }
    if (len > 1)
        shift(&result, len);
    return result;
}

uint1024_t mod_op(uint1024_t x, uint1024_t y) {
    if (compare_op(x, y) == -1)
        return x;
    uint1024_t current, other, zero = from_uint(0);
    int len;
    bool d;
    current = getlastpart_op(x, y.last_pos);
    other = getfirstpart_op(x, (x.last_pos - y.last_pos));
    len = x.last_pos - y.last_pos;
    if (compare_op(current, y) == -1) {
        current = getlastpart_op(x, y.last_pos + 1);
        other = getfirstpart_op(x, (x.last_pos - (y.last_pos + 1)));
        len = x.last_pos - (y.last_pos + 1);
    }
    while (len >= 0) {
        for (int i = current.last_pos - 1; i > 0; i--) {
            if (current.num[i] == 0)
                current.last_pos--;
            else
                break;
        }
        if (compare_op(current, y) != -1)
            current = small_mod_op(current, y);
        current = merge(current, getlastpart_op(other, 1));
        d = true;
        for (int i = other.last_pos - 1; i >= 0; i--) {
            if (other.num[i] != 0) {
                d = false;
                break;
            }
        }
        if ((compare_op(current, zero) == 0) && d) 
            break;
        other = getfirstpart_op(other, (other.last_pos - 1));
        len--;
    }
    shift(&current, -1);
    return current;
}

void printfold(uint1024_t x) {
    // for (int i = 128; i >= 0; i--)
    //     printf("%d ", x.num[i]);
    for (int i = (x.last_pos - 1); i >= 0; i--)
        printf("%d ", x.num[i]);
    printf("\n");
}

void printf_value(uint1024_t x) {
    uint8_t output[309];
    int pos = 0;
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
    // uint1024_t a = from_uint(123412423);
    // printfold(a);
    // shift(&a, -6);
    // printfold(a);
    // printfold(small_del_op(from_uint(1), from_uint(12)));

    // uint1024_t a = from_uint(1), two = from_uint(2);
    // for (int i = 1; i <= 1023; i++) {
    //     a = mult_op(a, two);
    // }
    // printfold(a);
    // printf_value(a);

    // uint1024_t a = from_uint(25), ten = from_uint(10), r;
    // printfold(a);
    // printfold(ten);
    // r = small_del_op(a, ten);
    // printfold(r);
    // printf("------------------------------\n");
    // r = del_op(a, ten);
    // printf("------------------------------\n");
    // printfold(r);
    return 0;
}