#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

const int Size = 128;

typedef struct uint1024_t
{
    uint8_t num[128];
    uint8_t last_pos;
} uint1024_t;

void printfold(uint1024_t x)
{
    for (int i = (x.last_pos - 1); i >= 0; i--)
        printf("%d ", x.num[i]);
    printf("\n");
}

uint8_t pos(uint8_t x, uint8_t y)
{
    if (x > y)
    {
        if ((x + 1) >= Size)
            return Size;
        else
            return (x + 1);
    }
    else
    {
        if ((y + 1) >= Size)
            return Size;
        else
            return (y + 1);
    }
}

uint1024_t from_uint(unsigned int x)
{
    uint1024_t result;
    if (x == 0)
    {
        result.last_pos = 1;
        for (int i = 0; i < Size; i++)
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
    for (int i = pos; i < Size; i++)
        result.num[i] = 0;
    return result;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint8_t buffer = 0;
    x.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < x.last_pos; i++)
    {
        uint8_t temp = buffer;
        buffer = (x.num[i] + y.num[i] + buffer) / 256;
        x.num[i] = (x.num[i] + y.num[i] + temp) % 256;
    }
    if (x.num[x.last_pos - 1] == 0)
        x.last_pos--;
    for (int i = x.last_pos; i < Size; i++)
        x.num[i] = 0;
    return x;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint8_t buffer = 0;
    x.last_pos = pos(x.last_pos, y.last_pos);
    for (int i = 0; i < x.last_pos; i++)
    {
        uint8_t temp = buffer;
        buffer = ((x.num[i] - y.num[i] - buffer) < 0 ? 1 : 0);
        x.num[i] = (256 + x.num[i] - y.num[i] - temp) % 256;
    }
    for (int i = (x.last_pos - 1); i > 0; i--)
    {
        if (x.num[i] == 0)
            x.last_pos--;
        else
            break;
    }
    for (int i = x.last_pos; i < Size; i++)
        x.num[i] = 0;
    return x;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    int buffer;
    uint1024_t result = from_uint(0), temp;
    for (int i = 0; i < x.last_pos; i++)
    {
        temp = from_uint(0);
        buffer = 0;
        for (uint8_t j = 0; j < y.last_pos; j++)
        {
            if ((i + j) < Size)
                temp.num[i + j] = (x.num[i] * y.num[j] + buffer) % 256;
            buffer = (x.num[i] * y.num[j] + buffer) / 256;
        }
        temp.last_pos = y.last_pos + i;
        if ((buffer != 0) && (temp.last_pos != Size))
        {
            temp.num[temp.last_pos] += (buffer % 256);
            temp.last_pos++;
        }
        for (uint8_t j = (temp.last_pos - 1); j >= 1; j--)
        {
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

void scanf_value(uint1024_t *x)
{
    *x = from_uint(0);
    char input[309];
    scanf("%s", input);
    int i = 0;
    while (i < 309)
    {
        if (input[i] != '\0')
            *x = add_op(mult_op(*x, from_uint(10)), from_uint(input[i] - '0'));
        else
            break;
        i++;
    }
}

uint1024_t shift(uint1024_t x, int pos)
{
    if (pos < 0)
    {
        pos = -pos;
        if (x.last_pos <= pos)
            x = from_uint(0);
        else
        {
            x.last_pos = x.last_pos - pos;
            for (int i = 0; i < x.last_pos; i++)
                x.num[i] = x.num[i + pos];
            for (int i = x.last_pos; i <= (Size - 1); i++)
                x.num[i] = 0;
            for (int i = x.last_pos - 1; i > 0; i--)
            {
                if (x.num[i] == 0)
                    x.last_pos--;
                else
                    break;
            }
        }
    }
    else
    {
        if ((x.last_pos + pos - 1) < Size)
            x.last_pos = x.last_pos + pos;
        for (int i = (x.last_pos - 1); i >= pos; i--)
            x.num[i] = x.num[i - pos];
        for (int i = (pos - 1); i >= 0; i--)
            x.num[i] = 0;
        for (int i = x.last_pos - 1; i > 0; i--)
        {
            if (x.num[i] == 0)
                x.last_pos--;
            else
                break;
        }
    }
    return x;
}

int compare_op(uint1024_t x, uint1024_t y)
{
    if (x.last_pos > y.last_pos)
        return 1;
    else if (x.last_pos < y.last_pos)
        return -1;
    else
    {
        for (int i = (x.last_pos - 1); i >= 0; i--)
        {
            if (x.num[i] > y.num[i])
                return 1;
            else if (x.num[i] < y.num[i])
                return -1;
        }
        return 0;
    }
}

uint1024_t del_op(uint1024_t x, uint1024_t y)
{
    uint1024_t zero = from_uint(0);
    uint1024_t res = zero, curValue = zero, cur;
    res.last_pos = x.last_pos;
    bool zerocheck = true;
    for (int i = x.last_pos - 1; i >= 0; i--)
    {
        curValue = shift(curValue, 1);
        curValue.num[0] = x.num[i];
        int x = 0, l = 0, r = 256;
        while (l <= r)
        {
            int m = (l + r) / 2;
            cur = mult_op(y, from_uint(m));
            if (compare_op(cur, curValue) != 1)
            {
                x = m;
                l = m + 1;
            }
            else
                r = m - 1;
        }
        if ((x == 0) && zerocheck)
            res.last_pos--;
        else
        {
            res.num[i] = x;
            zerocheck = false;
        }
        curValue = subtr_op(curValue, mult_op(y, from_uint(x)));
    }
    return res;
}

uint1024_t mod_op(uint1024_t x, uint1024_t y)
{
    uint1024_t curValue = from_uint(0), cur;
    for (int i = x.last_pos - 1; i >= 0; i--)
    {
        curValue = shift(curValue, 1);
        curValue.num[0] = x.num[i];
        int x = 0, l = 0, r = 256;
        while (l <= r)
        {
            int m = (l + r) / 2;
            cur = mult_op(y, from_uint(m));
            if (compare_op(cur, curValue) != 1)
            {
                x = m;
                l = m + 1;
            }
            else
                r = m - 1;
        }
        curValue = subtr_op(curValue, mult_op(y, from_uint(x)));
    }
    return curValue;
}

void longshort10(uint1024_t x, uint1024_t *delg, int *modg)
{
    uint1024_t zero = from_uint(0);
    if ((x.last_pos == 1) && (x.num[0] < 10))
    {
        *delg = zero;
        *modg = x.num[0];
    }
    *delg = zero;
    delg->last_pos = x.last_pos;
    int ost = 0, cur;
    bool zerocheck = true;
    for (int i = (delg->last_pos - 1); i >= 0; i--)
    {
        cur = ost * 256 + x.num[i];
        if ((cur < 10) && zerocheck)
            delg->last_pos--;
        else
        {
            delg->num[i] = cur / 10;
            zerocheck = false;
        }
        ost = cur % 10;
    }
    *modg = ost;
}

void printf_value(uint1024_t x)
{
    uint1024_t del, zero = from_uint(0);
    uint8_t output[309];
    int pos = 0, mod;
    while (compare_op(x, zero) == 1)
    {
        longshort10(x, &del, &mod);
        output[pos] = mod;
        x = del;
        pos++;
    }
    if (pos == 0)
        printf("0");
    for (int i = (pos - 1); i >= 0; i--)
        printf("%d", output[i]);
    printf("\n");
}