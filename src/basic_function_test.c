#include <morec.h>
#include <stdio.h>

void init_vector(vector_t* mimmo, size_t n) {
    for(float i = 0; i < n; i++) {
        vector_push_back(mimmo, i+0.123456f); 
    }
}

void print_vector(vector_t* mimmo) {
    for(size_t i = 0; i < vector_size(mimmo); i++) {
        printf("%f\n", vector_at(mimmo, float, i));
    }
}

iterator_t adv_beg_iterator(vector_t* mimmo, int64_t n) {
    iterator_t a = vector_begin(mimmo);
    iterator_advance(&a, n);

    return a;
}

iterator_t adv_end_iterator(vector_t* mimmo, int64_t n) {
    iterator_t a = vector_end(mimmo);
    iterator_advance(&a, n);

    return a;
}

int francesco_totti()
{
    vector_t franco = vector(float);

    // Nota personale:
    // L'handling degli iterator e' scomodo a merda porcoddio

    printf("\n-=-=-=-=-=-=-= PUSH_BACK TEST START =-=-=-=-=-=-=-\n");
    {
        for(float i = 0; i <= 0xFF; i++) {
            vector_push_back(&franco, i+0.385942f);
        }

        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=-=- PUSH_BACK TEST END -=-=-=-=-=-=-=-\n");
    // SUCCEED

    printf("\n-=-=-=-=-=-=-=- RESIZE TEST START -=-=-=-=-=-=-=-\n");
    {
        vector_resize(&franco, vector_size(&franco) - 0x7F);

        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=-=-= RESIZE TEST END =-=-=-=-=-=-=-=-\n");
    // SUCCEED

    printf("\n-=-=-=-=-=-= SINGLE ERASE TEST START =-=-=-=-=-=-\n");
    {
        iterator_t beg = vector_begin(&franco);
        iterator_advance(&beg, 100);
        vector_erase(&franco, beg);

        iterator_advance(&beg, -49);
        vector_erase(&franco, beg);

        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=- SINGLE ERASE TEST END -=-=-=-=-=-=-\n");
    // SUCCEED

    printf("\n-=-=-=-=-=-= RANGE ERASE TEST START =-=-=-=-=-=-\n");
    {
        iterator_t beg = vector_begin(&franco);
        iterator_t end = vector_end(&franco);

        iterator_advance(&beg, 50);

        vector_erase_range(&franco, beg, end);
        
        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=- RANGE ERASE TEST END -=-=-=-=-=-=-\n");
    // SUCCEED

    printf("\n-=-=-=-=-=-=-=- INSERT SINGLE TEST START -=-=-=-=-=-=-=-\n");
    {
        iterator_t end = vector_end(&franco);
        vector_insert_single(&franco, end, 455.28358f);
        end = vector_end(&franco);
        vector_insert_single(&franco, end, 999.28358f);

        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=-=-= INSERT SINGLE TEST END =-=-=-=-=-=-=-=-\n");
    // SUCCEED

    printf("\n-=-=-=-=-=-=-=- INSERT RANGE TEST START -=-=-=-=-=-=-=-\n");
    {
        vector_t madonna = vector(float);
        init_vector(&madonna, 50);

        iterator_t corbezzolo = adv_end_iterator(&franco, -1);

        vector_insert_range(&franco, corbezzolo, adv_beg_iterator(&franco, 10), adv_beg_iterator(&franco, 15));

        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=-=-= INSERT RANGE TEST END =-=-=-=-=-=-=-=-\n");
    // SUCCEED

    printf("\n-=-=-=-=-=-=-=- INSERT FILL TEST START -=-=-=-=-=-=-=-\n");
    {
        iterator_t asparago = vector_end(&franco);

        vector_insert_fill(&franco, asparago, 5, 5.555555f);

        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=-=-= INSERT FILL TEST END =-=-=-=-=-=-=-=-\n");
    // SUCCEED

    printf("\n-=-=-=-=-=-=-=- POP TEST START -=-=-=-=-=-=-=-\n");
    {
        for(int i = 0; i < 5; i++)
            vector_pop_back(&franco);

        print_vector(&franco);
    }
    printf("-=-=-=-=-=-=-=-= POP TEST END =-=-=-=-=-=-=-=-\n");
    // SUCCEED

    
}

/*
make PLATFORM=LINUX && ./bin/demo.elf
*/