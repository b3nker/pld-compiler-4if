int test(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int main() {
    test( ((((1 + 2) * 3 % 4) / 5 + (2 * 5 / 4) % (67 - 13))),
          ((((0 + 2) * 1 % (1 / 1) + 1 * (5 / 1)) % 7 - 0)),
          ~(~(~(~(~(~(~789)))))),
          (-(-(-(-(-(-567)))))),
          !(!(!(!(!(!(!1)))))),
          0 && 1 || 0 ^ 7 == 1 >= 0 < 0 <= 1 > 1 != 0 & 76 | 45
        );
    
    return 0;
}