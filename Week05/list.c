// Implement a list of numbers with an array of dynamic size

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // list of size 3
    int *list = malloc(3 * sizeof(int));
    if (list == NULL)
    {
        return 1;
    }

    // intialize list
    list[0] = 1;
    list[1] = 2;
    list[2] = 3;

    // list of size 4
    int *tmp = realloc(list, 4 * sizeof(int));
    if (tpm == NULL)
    {
        free(list);
        return 1;
    }
    list = tmp;
    list[3] = 4;
}