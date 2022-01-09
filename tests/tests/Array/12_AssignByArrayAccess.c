int main() {
    int a[16];
    a[0] = 1;
    a[a[0]] = 2;
    a[a[a[0]]] = 3;
    a[a[a[a[0]]]] = 123;
    return a[3];
}
