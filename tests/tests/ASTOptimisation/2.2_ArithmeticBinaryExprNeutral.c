int main() {
    int a;
    int b = 5;

    a = b + 0;
    a = 0 + b;

    a = b - 0;

    a = b * 1;
    a = 1 * b;

    a = b / 1;

    a = b % 1;

    a = 0 + b * 1 % 1 / 1 + 1 * b / 1 % b - 0;
    a = ((((0 + b) * 1 % (b / 1) + 1 * (b / 1)) % b - 0));

    return a;
}
