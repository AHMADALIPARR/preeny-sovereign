/* sovereign pure stub: rand/random always return 42 */
int rand(void)
{
    return 42;
}

int rand_r(unsigned int *seedp)
{
    return 42;
}

long random(void)
{
    return 42;
}
