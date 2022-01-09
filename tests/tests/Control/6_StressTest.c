int putchar(int c);

int alphabetReverse() {
    int i = 26;
    while (i) {
        i = i - 1;
        putchar(65 + i);
    }
    putchar(10);
    return 0;
}

int alphabet() {
    int j;
    for (j = 0; j - 26; j = j + 1) {
        putchar(97 + j);
    }
    putchar(10);
    return 0;
}

int printNumber(int num) {
    int ascii0 = 48;

    int chiffre = num % 10;
    int x = (num - chiffre) / 10;

    if (x) // recurse
        printNumber(x);
    putchar(ascii0 + chiffre);
    return 0;
}

int conditions() {
    int x = 0;
    int y = 1;
    int z = 0;
    if (x) {
        return 1;
    } else if (y) {
        return 2;
    } else if (z) {
        return 2;
    } else {
        return 3;
    }
    return 0;
}

int fib_rec(int n) {
    if (n * (n - 1))
        return fib_rec(n - 1) + fib_rec(n - 2);
    else
        return n;
}

int fib_iter(int n) {
    int a = 0, b = 1;

    if (n)
        ;
    else
        return 0;

    n = n + 1;

    int tmp;
    while (n = n - 1) {
        tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

int weirdControlStructures() {
    int i;

    if (1)
        ;

    if (1) {
    }

    if (1) {
    } else {
    }

    if (1)
        ;
    else {
    }

    if (1) {
    } else
        ;

    if (1)
        ;
    else
        ;

    if (1)
        ;
    else if (1)
        ;
    else
        ;

    if (1)
        ;
    else if (1)
        ;
    else if (1)
        ;
    else if (1)
        ;
    else
        ;

    while (0) {
    }

    while (0)
        ;

    for (i = 0; i - 10; i = i + 1) {
    }

    for (i = 0; i - 10; i = i + 1)
        ;

    for (i = 0; i - 10;)
        i = i + 1;

    for (i = 0; i - 10;) {
        i = i + 1;
    }
    for (i = 0;;) {
        i = i + 1;
        for (;;) {
            for (;;)
                for (; 1;)
                    for (;; 1)
                        for (1;;)
                            return 42;
        }
    }
    return 0;
}

int main() {
    alphabet();
    alphabetReverse();

    printNumber(2742);
    putchar(10);

    for (int k = 0; k - 30; k = k + 1) {
        printNumber(k);
        putchar(9);
        printNumber(fib_rec(k));
        putchar(9);
        printNumber(fib_iter(k));
        putchar(10);
    }

    return conditions() + weirdControlStructures();
}
