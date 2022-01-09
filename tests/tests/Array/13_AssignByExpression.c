int main() {
    int a[16], b = 7, c = 10, d = 5;
    a[b + c * d - 50] = 42;
    return a[7];
}
