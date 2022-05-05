#include <morec.h>
#include <stdio.h>
#include <assert.h>

void print_list(list_t *list) {
    iterator_t beg = list_begin(list);
    iterator_t end = list_end(list);

    while(iterator_distance(&beg, &end) > 0) {
        printf("%d\n", iterator_get(&beg, int));
        iterator_advance(&beg, 1);
    }

    printf("%d\n", list_back(int, list));
}

list_t init_list(size_t n) {
    list_t league_of_legends_merda = list(int);

    for(int i = 0; i < n; i++)
        list_push_back(&league_of_legends_merda, i);

    assert(list_size(&league_of_legends_merda) == n);

    return league_of_legends_merda;
}

/*
ITERATORS

TO-FIX:
- iterator_distance():
    Iterating from X to A doesn't go up to A but instead goes up to A-1

    Trying a condition such iterator_distance(X, A) >= 0 leads up to the following error:
    ./src/morec/container.c:9: list_iterator_next: Assertion `it->position != NULL' failed.

    :(
*/


void iterator_test_routine() {
    printf("\n-=-=-=-=-=-=-= ITERATOR CHECKS START =-=-=-=-=-=-=-\n");

    list_t league_of_legends_merda = init_list(50);

    {
        printf("Iterating from begin to end\n");

        iterator_t beg = list_begin(&league_of_legends_merda);
        iterator_t end = list_end(&league_of_legends_merda);

        while(iterator_distance(&beg, &end) != 0) {
            printf("%d\n", iterator_get(&beg, int));
            iterator_advance(&beg, 1);
        }

        printf("Iterating from begin to end\n");
    }

    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

    {
        printf("Iterating from end to begin (using reverse iterator)\n");

        iterator_t rbeg = list_rbegin(&league_of_legends_merda);
        iterator_t rend = list_rend(&league_of_legends_merda);

        while(iterator_distance(&rbeg, &rend) != 0) {
            printf("%d\n", iterator_get(&rbeg, int));
            iterator_advance(&rbeg, 1);
        }

        printf("Iterating from end to begin (using reverse iterator)\n");
    }   

    printf("-=-=-=-=-=-=-=- ITERATOR CHECKS END -=-=-=-=-=-=-=-\n");
}

// -=-=-=-=-=-=-=-= MODIFIERS TEST =-=-=-=-=-=-=-=-

void test_erase() {
    list_t spaghetto = init_list(50);

    iterator_t it = list_begin(&spaghetto);

    list_erase(&spaghetto, it);

    assert(list_size(&spaghetto) == 49);
    print_list(&spaghetto);
}

void test_erase_range() {
    // TO-DO:
    // Test range for invalid iterators (plz fix sigsegv :((( )
    list_t spaghetto = init_list(50);

    iterator_t beg = list_begin(&spaghetto);
    iterator_t end = list_end(&spaghetto);
    iterator_advance(&beg, 35);

    list_erase_range(&spaghetto, beg, end);

    assert(list_size(&spaghetto) == 35);
    print_list(&spaghetto);
}

void test_assign_fill() {
    list_t spaghetto = init_list(50);

    list_assign_fill(&spaghetto, 100, 0xbeef);

    assert(list_size(&spaghetto) == 100);
}

void test_assign_range() {
    list_t spaghetto = init_list(50);

    list_t ramen = init_list(100);
    iterator_t beg = list_begin(&ramen);
    iterator_t end = list_end(&ramen);

    list_assign_range(&spaghetto, beg, end);   

    assert(list_size(&spaghetto) == 100);     
}

void test_push_front() {
    list_t spaghetto = init_list(50);

    for(int i = -1; i >= -50; i--)
        list_push_front(&spaghetto, i);

    assert(list_size(&spaghetto) == 100);
}

void test_pop_front() {
    list_t spaghetto = init_list(50);

    for(int i = 0; i < 25; i++) {
        list_pop_front(&spaghetto);
    }

    assert(list_size(&spaghetto) == 25);
    assert(list_front(int, &spaghetto) == 25);
}

void test_pop_back() {
    list_t spaghetto = init_list(50);

    for(int i = 0; i < 25; i++) {
        list_pop_back(&spaghetto);
    }

    assert(list_size(&spaghetto) == 25);
    assert(list_back(int, &spaghetto) == 24);
}

void test_insert_single() {
    list_t spaghetto = init_list(50);

    iterator_t it = list_begin(&spaghetto);
    list_insert_single(&spaghetto, it, 100);

    print_list(&spaghetto);

    assert(list_size(&spaghetto) == 51);
    assert(list_front(int, &spaghetto) == 100);
}

void test_insert_fill() {
    list_t spaghetto = init_list(50);

    iterator_t it = list_begin(&spaghetto);

    list_insert_fill(&spaghetto, it, 50, 0xbeef);

    print_list(&spaghetto);

    assert(list_size(&spaghetto) == 100);

    iterator_t medusa = list_begin(&spaghetto);
    iterator_advance(&medusa, 25);
    assert(iterator_get(&medusa, int) == 0xbeef);

    assert(list_back(int, &spaghetto) == 49);
}

void test_insert_range() {
    list_t spaghetto = init_list(50);
    list_t rizla = init_list(25);

    iterator_t dest_it = list_begin(&spaghetto);
    iterator_t src_it = list_begin(&rizla);
    iterator_t src_end = list_end(&rizla);

    list_insert_range(&spaghetto, dest_it, src_it, src_end);

    print_list(&spaghetto);

    assert(list_size(&spaghetto) == 75);

    iterator_t kraken = list_begin(&spaghetto);
    iterator_advance(&kraken, 25);
    assert(iterator_get(&kraken, int) == 0);

    assert(list_back(int, &spaghetto) == 49);
}

void test_swap() {
    list_t spaghetto = init_list(50);
    list_t panzerkampfwagen = list(int);
    for(int i = 99; i >= 0; i--) {
        list_push_back(&panzerkampfwagen, i);
    }

    list_swap(&spaghetto, &panzerkampfwagen);

    assert(list_size(&spaghetto) == 100);
    assert(list_size(&panzerkampfwagen) == 50);

    assert(list_front(int, &spaghetto) == 99);
    assert(list_back(int, &panzerkampfwagen) == 49);
}

void test_resize() {
    list_t spaghetto = init_list(50);

    list_resize(&spaghetto, 30);

    assert(list_size(&spaghetto) == 30);
    assert(list_back(int, &spaghetto) == 29);
}   

void test_clear() {
    list_t spaghetto = init_list(50);

    list_clear(&spaghetto);

    assert(list_size(&spaghetto) == 0);
}

// -=-=-=-=-=-=-=-= MODIFIERS TEST =-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-= OPERATIONS TEST =-=-=-=-=-=-=-=-

void test_splice_entire() {
    list_t spaghetto = init_list(50);
    list_t oii2022 = list(int);
    for(int i = 98; i >= 0; i -= 2) {
        list_push_back(&oii2022, i);
    }

    iterator_t it = list_begin(&spaghetto);
    iterator_advance(&it, 30);

    list_splice_entire(&spaghetto, it, &oii2022);

    assert(list_size(&spaghetto) == 100);
    assert(list_size(&oii2022) == 0);

    iterator_t medusa = list_begin(&spaghetto);
    iterator_advance(&medusa, 30);
    assert(iterator_get(&medusa, int) == 98);

    assert(list_back(int, &spaghetto) == 49);
}

void test_splice_single() {
    list_t spaghetto = init_list(50);
    list_t rumeno = list(int);

    for(int i = 0; i < 100; i += 2) {
        list_push_back(&rumeno, i);
    }

    iterator_t src_it = list_begin(&rumeno);
    iterator_advance(&src_it, 30);

    iterator_t dest_it = list_begin(&spaghetto);
    iterator_advance(&dest_it, 5);

    list_splice_single(&spaghetto, dest_it, &rumeno, src_it);

    assert(list_size(&spaghetto) == 51);
    assert(list_size(&rumeno) == 49);

    iterator_t apollo = list_begin(&spaghetto);
    iterator_advance(&apollo, 5);
    assert(iterator_get(&apollo, int) == 60);
}

void test_splice_range() {
    list_t spaghetto = init_list(50);
    list_t rumeno = list(int);
    for(int i = 0; i < 100; i += 2) {
        list_push_back(&rumeno, i);
    }

    iterator_t src_beg = list_begin(&rumeno);
    iterator_advance(&src_beg, 30); // 60

    iterator_t src_end = list_begin(&rumeno);
    iterator_advance(&src_end, 40); // 80

    iterator_t dest_it = list_begin(&spaghetto);
    iterator_advance(&dest_it, 5);

    // WRONG IMPLEMENTATION OF RANGE!
    // MISSING SOURCE LIST PARAMETER
}

// -=-=-=-=-=-=-=-= OPERATIONS TEST =-=-=-=-=-=-=-=-

/*
LISTS

TO-FIX:
Modifiers:
    - assign
        > Starts overwriting from the last element and not the first.
        > Doesn't modify the size accordingly as said by STL.

    - erase (range)
        > leads to SIGSEGV (bruh?)

    - insert (single)
        > Overwrites element at position <it> instead of inserting the element before position <it>

    - insert (fill)
        > Only overwrites element at position <it> instead of inserting <n> elements before position <it>

    - insert (range)
        > Only overwrites element at position <it> with the second-last element of the range instead of inserting the range

    - swap
        > bruh why does this asertion even get triggered?
        ./src/morec/list.c:291: list_swap: Assertion `list1->elem_size == list2->elem_size' failed.

    - resize
        > leads to SIGSEGV (bruh?)

    - clear
        > leads to SIGSEGV (bruh?)

Operations:
    - splice (entire)
        > Doesn't modify the size of the destination list (can't proceed with other tests)
    
    - splice (single)
        > Doesn't modify the size of the destination list (can't proceed with other tests)

    - splice (range)
        > Missing source list parameter...

*/

int main() {

    // -=-=-=-=-=-=-=- POSITIVE TESTS -=-=-=-=-=-=-=-
    /*
    iterator_test_routine();

    test_erase();

    test_push_front();

    test_pop_front();
    test_pop_back();
    */
    // -=-=-=-=-=-=-=- POSITIVE TESTS -=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=- NEGATIVE TESTS -=-=-=-=-=-=-=-
    /*
    test_assign_fill();
    test_assign_range();

    test_erase_range();

    test_insert_single();
    test_insert_fill();
    test_insert_range();

    test_swap();

    test_resize();

    test_clear();

    test_splice_entire();
    test_splice_single();
    test_splice_range();
    */
    // -=-=-=-=-=-=-=- NEGATIVE TESTS -=-=-=-=-=-=-=-
}

/*
make PLATFORM=LINUX && ./bin/demo.elf
*/