#include <morec.h>
#include <stdio.h>
#include <assert.h>

int main()
{
    list_t list = list(int);
    list_assign_fill(&list, 50, 10);

    list_t list2 = list(int);
    list_assign_range(&list2, list_begin(&list), list_end(&list));

    iterator_t end = list_end(&list2);
    for (iterator_t it = list_begin(&list2); iterator_distance(&it, &end) != 0; iterator_advance(&it, 1))
    {
        printf("%d\n", iterator_get(&it, int));
    }
}

/*
make PLATFORM=LINUX && ./bin/demo.elf
*/