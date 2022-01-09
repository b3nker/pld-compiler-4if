int main() {
    int a = 'z';
    if (a == 'x') {
        return 1;
    } else if (a == 'y') {
        return 2;
    } else if (a == 'z') {
        return 3;
    } else if (a == 't') {
        return 4;
    } else {
        return 5;
    }
    return 0; // dead code
}
