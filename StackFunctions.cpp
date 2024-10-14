#include "StackFunctions.h"

int StackCtor(Stk_t* stk)
{
    assert(stk);

    stk->data = (stk_el_t*) ((char*) calloc(1, CAPACITY_MIN * sizeof(stk_el_t) ON_BIRDS(+ 2 * CANARY_ADDED_SIZE)));

    if (stk->data == NULL) return -1;

    stk->sz       = 0;
    stk->capacity = CAPACITY_MIN;
    stk->err_code = NO_ERR;

    #ifdef CANARIES_MODE
        assert(stk->data);

        stk->data = (stk_el_t*) ((char*) stk->data + CANARY_ADDED_SIZE);

        *((canary_t*)((char*) stk->data - CANARY_ADDED_SIZE)) = CANARY;

        *((canary_t*)(stk->data + stk->capacity)) = CANARY;
    #endif

    ON_BIRDS(stk->canary2 = CANARY;)

    //FillPoison(stk);

    #ifdef HASH_MODE
        assert(stk->data);

        stk->data_hash = CalculateHash((const char*) stk->data, CAPACITY_MIN * sizeof(stk_el_t));

        stk->stk_hash  = CalculateHash((char*) stk + STK_PTR_SHIFT, STK_SIZE);
    #endif

    HASH_VERIFY(stk)

    return 0;
}

void StackDtor(Stk_t* stk)
{
    assert(stk);

    free((char*) stk->data ON_BIRDS(- CANARY_ADDED_SIZE));

    stk->data     = NULL;
    stk->capacity = CAPACITY_MIN;
    stk->sz       = 0;
}

int FillPoison (Stk_t* stk)
{
    STACK_VERIFY(stk)

    for (size_t i = stk->sz; i < stk->capacity; ++i) {
        (stk->data)[i] = POISON;
    }

    return 0;
}

void StackPush(Stk_t* stk, stk_el_t el)
{
    HASH_VERIFY(stk)
    STACK_VERIFY(stk)

    if (stk->sz == stk->capacity) {

        stk->data = (stk_el_t*) realloc((char*) stk->data ON_BIRDS(- CANARY_ADDED_SIZE),
                                        (stk->sz * 2 + (stk->sz == 0) * CAPACITY_MIN) * sizeof(stk_el_t)
                                        ON_BIRDS(+ 2 * CANARY_ADDED_SIZE));

        #ifdef CANARIES_MODE
            assert(stk->data);
            stk->data = (stk_el_t*) ((char*) stk->data + CANARY_ADDED_SIZE);
            assert(stk->data);
        #endif

        if (stk->capacity == 0) stk->capacity = CAPACITY_MIN;
        else stk->capacity *= 2;

        ON_BIRDS(*((canary_t*) (stk->data + stk->capacity)) = CANARY;)

        //FillPoison(stk);

        STACK_VERIFY(stk)
    }

    (stk->data)[(stk->sz)++] = el;

    #ifdef HASH_MODE
        stk->data_hash = CalculateHash((const char*) stk->data, stk->capacity * sizeof(stk_el_t));
        stk->stk_hash  = CalculateHash((const char*) stk + STK_PTR_SHIFT, STK_SIZE);
    #endif

    STACK_VERIFY(stk)
}

stk_el_t StackPop(Stk_t* stk)
{
    HASH_VERIFY(stk)
    STACK_VERIFY(stk)

    if (stk->sz == 0) {
        fprintf(stderr, RED "Error: calling pop() for empty stack\n" COLOUR_RESET);
        return POISON;
    } else {
        stk_el_t val = (stk->data)[--(stk->sz)];
        //(stk->data)[stk->sz] = POISON;

        if (stk->capacity > CAPACITY_MIN && stk->sz == stk->capacity / 4) {

            stk->data = (stk_el_t*) realloc((char*) stk->data ON_BIRDS(- CANARY_ADDED_SIZE),
                                            sizeof(stk_el_t) * stk->capacity / 2 ON_BIRDS(+ 2 * CANARY_ADDED_SIZE));
            #ifdef CANARIES_MODE
                assert(stk->data);
                stk->data = (stk_el_t*) ((char*) stk->data + CANARY_ADDED_SIZE);
                assert(stk->data);
            #endif

            stk->capacity /= 2;

            ON_BIRDS(*((canary_t*) (stk->data + stk->capacity)) = CANARY;)
        }

        #ifdef HASH_MODE
            stk->data_hash = CalculateHash((const char*) stk->data, stk->capacity * sizeof(stk_el_t));
            stk->stk_hash  = CalculateHash((const char*) stk + STK_PTR_SHIFT, STK_SIZE);
        #endif

        STACK_VERIFY(stk)

        return val;
    }
}

int StackError(Stk_t* stk)
{
    assert(stk);
    if (stk->err_code == NO_ERR) {
        if (stk->data == NULL) stk->err_code = DATA_ERR;

        else if (stk->sz > stk->capacity) stk->err_code = SIZE_ERR;

        #ifdef CANARIES_MODE

            else if (stk->canary1 != CANARY || stk->canary2 != CANARY) stk->err_code = CANARY_ERR;

            else if (*((canary_t*)((char*) stk->data - CANARY_ADDED_SIZE)) != CANARY ||
                     *((canary_t*)(stk->data + stk->capacity))             != CANARY) stk->err_code = CANARY_ERR;

        #endif
    }

    return stk->err_code;
}

#ifdef HASH_MODE
    int HashCheck(Stk_t* stk)
    {
        assert(stk);

        if (stk->err_code == NO_ERR) {
            if (CalculateHash((const char*)stk->data, stk->capacity * sizeof(stk_el_t)) != stk->data_hash)
                stk->err_code = DATA_HASH_ERR;

            else if (CalculateHash((char*)stk + STK_PTR_SHIFT, STK_SIZE) != stk->stk_hash) {
                stk->err_code = STK_HASH_ERR;
            }
        }

        /*printf("data hash calculated: %llx\n", CalculateHash((const char*)stk->data, stk->capacity * sizeof(stk_el_t)));
        printf("stk hash calculated:  %llx\n", CalculateHash((const char*)stk + STK_PTR_SHIFT, STK_SIZE));
        */
        return stk->err_code;
    }

    long long int CalculateHash (const char* buffer, size_t size)
    {
        assert(buffer);

        long long int hash = 0x5381;

        for (size_t i = 0; i < size; ++i) {
            hash = hash * 31 ^ buffer[i];
        }

        return hash;
    }
#endif

void StackDump(Stk_t* stk ON_DEBUG(, const char* file_name, const int line, const char* func_name))
{
    printf("\nStk_t [0x%p]", stk);
    assert(stk);

    ON_DEBUG(printf(" born at %s:%d, named \"%s\"\n", stk->file, stk->line, stk->name);)
    ON_DEBUG(printf("called from %s:%d (%s)\n", file_name, line, func_name);)

    ON_BIRDS(printf("{\n  canary1 = %llx\n  ", stk->canary1);)

    ON_HASH(printf("Stk  hash: %llx\n  ", stk->stk_hash);
            printf("Data hash: %llx\n  ", stk->data_hash);)

    printf("capacity = %lld\n  size     = %lld\n  data [0x%p]", stk->capacity, stk->sz, stk->data);
    assert(stk->data);

    printf(":\n  {\n");

    ON_BIRDS(printf("    woodpecker1 = %llx\n", *((canary_t*)((char*) stk->data - CANARY_ADDED_SIZE)));)

    for (size_t i = 0; i < stk->capacity; ++i) {
        if ((stk->data)[i] != POISON) {
            printf("   *[%lld] = %d\n", i, (stk->data)[i]);
        } else {
            printf("    [%lld] = %d (POISON)\n", i, (stk->data)[i]);
        }
    }

    ON_BIRDS(printf("    woodpecker2 = %llx\n", *((canary_t*)(stk->data + stk->capacity)));)

    printf("  }\n  ");
    ON_BIRDS(printf("canary2 = %llx\n}\n", stk->canary2);)

    if (stk->err_code != NO_ERR) PrintError(stk->err_code);
}

void PrintError(int error_code)
{
    switch (error_code)
    {
        case DATA_ERR:
            fprintf(stderr, RED "Error: address of data is NULL\n" COLOUR_RESET);
            break;

        case SIZE_ERR:
            fprintf(stderr, RED "Error: size > capacity\n" COLOUR_RESET);
            break;

        case CANARY_ERR:
            fprintf(stderr, RED "Canary error\n" COLOUR_RESET);
            break;

        case WOODPECKER_ERR:
            fprintf(stderr, RED "Woodpecker error\n" COLOUR_RESET);
            break;

        case DATA_HASH_ERR:
            fprintf(stderr, RED "Data hash error\n" COLOUR_RESET);
            break;

        case STK_HASH_ERR:
            fprintf(stderr, RED "Stk hash error\n" COLOUR_RESET);
            break;

        default:
            fprintf(stderr, RED "Unknown error %d\n" COLOUR_RESET, error_code);
    }
}
