int func(int a, int b, int c, int d, int e, int f, int g, int h, int i) {
    return a + 2 * b + 4 * c + 8 * d + 16 * e + 32 * f + 64 * g + 128 * h +
           256 * i;
}

int main() { return func(3, 5, 8, 13, 21, 34, 55, 89, 144) % 255; }
