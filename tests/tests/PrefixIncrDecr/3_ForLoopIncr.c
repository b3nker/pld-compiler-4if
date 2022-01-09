int main() {
    int x = 2;
    for (int i = 1; i < 100; ++i) {
        x = 3 * i % 255;
    }
    return x;
}
