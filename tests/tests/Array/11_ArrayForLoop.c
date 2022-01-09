int main() {
    int arr[32];

    arr[0] = 1;
    arr[1] = 1;
    for (int i = 2; i - 32; i = i + 1) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }

    return arr[31];
}
