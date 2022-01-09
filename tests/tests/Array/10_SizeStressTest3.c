int main() {
    int array[262144];
    array[0] = 0;
    array[1] = 1;
    array[262142] = 262142;
    array[262143] = 262143;
    return array[262143];
}