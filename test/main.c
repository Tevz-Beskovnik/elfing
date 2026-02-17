void testing2(int t)
{
    int j = 0;
    j += t;
}

int test()
{
    return 1;
}

void _start()
{
    int t = test();
    testing2(t);
}
