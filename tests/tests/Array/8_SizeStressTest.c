int main() {
    int array[1024];
    array[0] = 0;
    array[1] = 1;
    array[1022] = 1022;
    array[1023] = 1023;
    return array[1023];
}