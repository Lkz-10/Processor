#ifndef __GLOBALS_STK_H__
#define __GLOBALS_STK_H__

//#define DEBUG
//#define HASH_MODE
//#define CANARIES_MODE

#include <stdlib.h>

typedef double        stk_el_t;
typedef long long int canary_t;

const stk_el_t POISON       = -79173490302.102;
const size_t   CAPACITY_MIN = 10;
const size_t   STK_SIZE     = sizeof(int) + 2 * sizeof(size_t) + sizeof(stk_el_t*);

#ifdef CANARIES_MODE
    const canary_t CANARY            = 0xEDADEDA;
    const size_t   CANARY_ADDED_SIZE = sizeof(CANARY) + (8 - sizeof(CANARY) % 8) % 8;
#endif

enum Error_codes
{
    NO_ERR         =  0,
    DATA_ERR       =  1,
    SIZE_ERR       =  2,
    CANARY_ERR     =  4,
    WOODPECKER_ERR =  8,
    STK_HASH_ERR   = 16,
    DATA_HASH_ERR  = 32
};

#ifdef CANARIES_MODE
    #define ON_BIRDS(...) __VA_ARGS__
#else
    #define ON_BIRDS(...)
#endif

#ifdef HASH_MODE
    #define ON_HASH(...) __VA_ARGS__
    #define HASH_VERIFY(var)                                   \
        if (HashCheck(var) != NO_ERR) {                        \
            /*StackDump(var, __FILE__, __LINE__, __func__);*/  \
            PrintError(HashCheck(var));                        \
            StackDtor(var);                                    \
            assert(0);                                         \
        }
#else
    #define ON_HASH(...)
    #define HASH_VERIFY(var)
#endif

#ifdef DEBUG
    #define INIT(var) ON_BIRDS(CANARY, )#var, __FILE__, __LINE__
    #define ON_DEBUG(...) __VA_ARGS__
    #define STACK_VERIFY(var)                                  \
        if (StackError(var) != NO_ERR) {                       \
            StackDump(var, __FILE__, __LINE__, __func__);      \
            StackDtor(var);                                    \
            assert(0);                                         \
        }
#else
    #define INIT(var)
    #define ON_DEBUG(...)
    #define STACK_VERIFY(var)
#endif

ON_HASH(const size_t STK_PTR_SHIFT = ON_BIRDS(CANARY_ADDED_SIZE +)
                                     ON_DEBUG(2 * sizeof(const char*) + sizeof(const int) +)
                                     2 * sizeof(long long int) + 4;)

struct Stk_t
{
    ON_BIRDS(canary_t canary1;)

    #ifdef DEBUG
        const char*   name;
        const char*   file;
        const int     line;
    #endif

    #ifdef HASH_MODE
        long long int data_hash;
        long long int stk_hash;
    #endif

    int       err_code;
    size_t    sz;
    size_t    capacity;
    stk_el_t* data;

    ON_BIRDS(canary_t canary2;)
};

#endif
