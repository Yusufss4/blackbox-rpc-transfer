#include <stdio.h>

int main()
{
    int num1, num2, result;

    scanf("%d", &num1);
    scanf("%d", &num2);

    if (num1 < 0 || num2 < 0)
    {
        fprintf(stderr, "Negative numbers not allowed for this blackbox.\n");
        return 1;
    }

    result = num1 * num2;

    fprintf(stdout, "%d\n", result);

    return 0;
}