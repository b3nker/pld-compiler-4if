int main() {
    int array[4];
    array[0] = 0;
    array[1] = 1;
    array[2] = 2 + array[0];
    array[3] = 3 * array[1];

    return array[0] + array[1] + array[2] + array[3];
}
