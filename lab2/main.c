#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <uint1024_t.h>

int main() {
    uint1024_t x = from_uint(3156), y = from_uint(154);
    printf_value(add_op(x, y));
    printf_value(subtr_op(x, y));
    printf_value(mult_op(x, y));
    printf_value(del_op(x, y));
    printf_value(mod_op(x, y));
    uint1024_t a = from_uint(1), b = from_uint(2);
    for (int i = 1; i <= 1023; i++)
        a = mult_op(a, b);
    printf_value(a);
    scanf_value(&a);
    printf_value(a);
    return 0;
}