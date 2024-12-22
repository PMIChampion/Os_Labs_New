extern "C" float E(int x) {
    float sum = 0.0f;
    int factorial = 1;
    for (int n = 0; n <= x; ++n) {
        if (n > 0){
            factorial *= n;
        }
        sum += 1.0f / factorial;
    }
    return sum;
}
