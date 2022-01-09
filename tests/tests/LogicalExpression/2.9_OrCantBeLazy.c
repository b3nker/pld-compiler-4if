int main() {
    int a = 0;
    int b = 0;
    int c = a || (b = 3);
    return b;
}
