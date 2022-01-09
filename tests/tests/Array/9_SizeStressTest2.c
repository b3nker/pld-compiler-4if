int main() {
    int array[65535];
    array[0] = 0;
    array[1] = 1;
    array[65533] = 65533;
    array[65534] = 65534;
    return array[65534];
}