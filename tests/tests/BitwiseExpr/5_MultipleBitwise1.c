int main() {
    int a = 8, b = 10;
    int c = a | 5;
    int d = c ^ b & 12 ^ a;
    int e = d | a ^ ~b & c ^ a;
    return e;
}
