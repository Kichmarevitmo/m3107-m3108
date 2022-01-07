#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//Далее будет приведена реализация длинной арифметики, использующая размер разряда 10^9, и массив длинной 35 для хранения разрядов. 
//Таким образом, эта реализация позволяет быстро работать с числами до примерно 2^1046
#define UINT1024_T
#define UINT1024_BASE (unsigned int) 1e9 //размер разряда
#define UINT1024_SIZE 35 //количество вмещаемых разрядов

typedef unsigned char uint1024_length;

typedef struct {
    uint32_t ints[35];
} uint1024_t;
/*число храним в достаточно длинном массиве*/

uint1024_t from_uint(unsigned int x); // генерация из числа

uint1024_t add_op(uint1024_t x, uint1024_t y); // сложение

uint1024_t subtr_op(uint1024_t x, uint1024_t y); //вычетание

uint1024_t mult_op(uint1024_t x, uint1024_t y); // умножение

void printf_value(uint1024_t x); // вывод в стандартный поток вывода

void scanf_value(uint1024_t *x); // чтение из стандартного потока ввода


int main() {
    printf("Enter unint1024 x:\n>");
    uint1024_t x;
    scanf_value(&x);
    printf("Enter unint1024 y:\n>");
    uint1024_t y;
    scanf_value(&y);

    printf("x =");
    printf_value(x);
    printf("y =");
    printf_value(y);

    printf("x + y = ");
    printf_value(add_op(x, y));
    printf("x - y = ");
    printf_value(subtr_op(x, y));
    printf("x * y = ");
    printf_value(mult_op(x, y));

    return 0;
}

uint1024_t from_uint(unsigned int x) { // генерация из числа
    uint1024_t temp;
    temp.ints[0] = x % UINT1024_BASE;
    temp.ints[1] = x / UINT1024_BASE;
    for(int i = 2; i < UINT1024_SIZE; i++) {
        temp.ints[i] = 0;
    }
    return temp;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) { // сложение
    uint1024_t temp;
    for(int i = 1; i < UINT1024_SIZE; i++) {
        temp.ints[i] = 0;
    }
    temp.ints[0] = (x.ints[0] + y.ints[0]) % UINT1024_BASE;
    bool carry_bit = (x.ints[0] + y.ints[0]) / UINT1024_BASE;
    int i;
    for (i = 1; i < UINT1024_SIZE; i++) {
        temp.ints[i] = (x.ints[i] + y.ints[i] + carry_bit) % UINT1024_BASE;
        carry_bit = (x.ints[i] + y.ints[i] + carry_bit) / UINT1024_BASE;
    }
    return temp;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) { //вычетание
    uint1024_t temp;
    for(int i = 0; i < UINT1024_SIZE; i++) {
        temp.ints[i] = 0;
    }
    bool carry_bit = 0;
    if (x.ints[0] < y.ints[0]) {
        carry_bit = 1;
        temp.ints[0] = UINT1024_BASE;
    }
    temp.ints[0] += x.ints[0] - y.ints[0];
    int i;
    for (i = 1; i < UINT1024_SIZE; i++) {
        if (x.ints[i] < y.ints[i] + carry_bit) {
            temp.ints[i] = UINT1024_BASE + x.ints[i] - y.ints[i] - carry_bit;
            carry_bit = 1;
        } else {
            temp.ints[i] += x.ints[i] - y.ints[i] - carry_bit;
            carry_bit = 0;
        }
    }
    return temp;
}

int karatsuba(uint1024_t *a, uint1024_t *b, uint1024_t *c, int n) {
    if (n <= 4)  {// 10000000
        unsigned long long temp[UINT1024_SIZE];
        int len = n;
        for (int j = 0; j < UINT1024_SIZE; j++) {
            temp[j] = c->ints[j];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[i + j] += ((unsigned long long) a->ints[i]) * ((unsigned long long) b->ints[j]);
            }
        }
        uint32_t carry = 0;
        for (int j = 0; j < UINT1024_SIZE; j++) {
            c->ints[j] = (temp[j] + carry) % UINT1024_BASE;
            carry = (temp[j] + carry) / UINT1024_BASE;
            if (c->ints[j] > 0 && j + 1 > n) {
                len = j + 1;
            }
        }
        return len;
    } else {
        int k1 = (n + 1) / 2;
        int k2 = n / 2;
        uint1024_t l, r, t, a1, a2, b1, b2, p1_t, p2_t;
        for (int i = 0; i < UINT1024_SIZE; i++) {
            l.ints[i] = 0;
            r.ints[i] = 0;
            t.ints[i] = 0;
            a1.ints[i] = 0;
            a2.ints[i] = 0;
            b1.ints[i] = 0;
            b2.ints[i] = 0;
            p1_t.ints[i] = 0;
            p2_t.ints[i] = 0;
        }
        for (int i = 0; i < k1; i++) {
            a1.ints[i] = a->ints[i];
            b1.ints[i] = b->ints[i];
        }
        for (int i = 0; i < k2; i++) {
            a2.ints[i] = a->ints[k1 + i];
            b2.ints[i] = b->ints[k1 + i];
        }
        l = add_op(a1, a2);
        r = add_op(b1, b2);

        karatsuba(&l, &r, &t, k1); // считает t
        int nk1 = karatsuba(&a1, &b1, &p1_t, k1); // считает p1

        int nk2 = karatsuba(&a2, &b2, &p2_t, k2); // считает p2

        l = subtr_op(t, p1_t);
        l = subtr_op(l, p2_t);

        int nk = 0;
        for (int i = 0; i < UINT1024_SIZE; i++) {
            if (l.ints[i] > 0) {
                nk = i + 1;
            }
        }


        for (int i = 0; i < UINT1024_SIZE; i++) {
            r.ints[i] = 0;
            c->ints[i] = 0;
        }
        for(int i = 0; i < nk1; i++) {
            c->ints[i] = p1_t.ints[i];
        }
        for(int i = 0; i < nk2; i++) {
            r.ints[k1 + i] = p2_t.ints[i];
        }
        (*c) = add_op(*c, r);

        for (int i = 0; i < UINT1024_SIZE; i++) {
            p2_t.ints[i] = 0;
        }
        for(int i = 0; i < nk && k1 + k2 + i < UINT1024_SIZE; i++) {
            p2_t.ints[k1 + k2 + i - 1] = l.ints[i];
        }
        (*c) = add_op(*c, p2_t);

        int len = n;
        for (int i = n; i < UINT1024_SIZE; i++) {
            if (c->ints[i] > 0) {
                len = i + 1;
            }
        }
        return len;
    }
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t res;
    for (int i = 0; i < UINT1024_SIZE; i++) {
        res.ints[i] = 0;
    }
    karatsuba(&x, &y, &res, UINT1024_SIZE);
    return res;
}

void printf_value(uint1024_t x) {
    bool flag = false;
    /* flag - флажок, который при выводе числа
              помогает пропустить начальные нули в числе
              начальные нули - это разряды, до которых
              число не доросло */
    for (int i = 0; i < UINT1024_SIZE; i++) {
        if (x.ints[UINT1024_SIZE - i - 1] != 0) {
            if (!flag) {
                printf(" %u", x.ints[UINT1024_SIZE - i - 1]);
                flag = true;
            } else {
                printf(" %.9u", x.ints[UINT1024_SIZE - i - 1]);
            }
        } else {
            if (flag) {
                printf(" 000000000");
            }
        }
    }
    if (!flag) {
        printf(" 0");
    }
    printf("\n");
}

void scanf_value(uint1024_t *x) {
    for(int i = 0; i < UINT1024_SIZE; i++) {
        x->ints[i] = 0;
    }
    char *number = (char *) calloc(1000, sizeof(char));
    scanf("%s", number); //без проверки на корректность ввода
    int cur = 0;
    int i = strlen(number) - 1;
    for (;; i -= 9, cur++) {
        char *temp = (char *) calloc(10, sizeof(char));
        temp[9] = '\0';
        if(cur >= UINT1024_SIZE) {
            printf("Error: uint1024 overflow");
            break;
        }
        if(i < 8) {
            strncpy(temp, number, i + 1);
            x->ints[cur] = atoi(temp);
            free(temp);
            break;
        }
        strncpy(temp, number + i - 8, 9);
        x->ints[cur] = atoi(temp);
        free(temp);
    }
    free(number);
}
