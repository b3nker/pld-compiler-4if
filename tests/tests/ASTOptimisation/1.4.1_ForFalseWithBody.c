int main() {
    int i = 5;
    for (i = 0; 0; i = i+1) {
        i = 10;
    }
    return i;
}
