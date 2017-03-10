int nesimo_primo(long int n, long int primos[]) {
    int i;

    for(i = 0; primos[i]*primos[i] <= n; i++)
        if (n % primos[i] == 0)
            return 0;

    return 1;

}

