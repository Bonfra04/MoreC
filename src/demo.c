#include <morec.h>
#include <stdio.h>
#include <assert.h>
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

int main()
{
    printf("CIAO");
    vector_t franco = vector(float);
    init_vector(&franco, 50);

    iterator_t barbabietola = adv_end_iterator(&franco, -5);

    
    print_vector(&franco);
}

/*
make PLATFORM=LINUX && ./bin/demo.elf
*/