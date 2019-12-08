#ifndef CO_ASYNC_H
#define CO_ASYNC_H

//Includes
#include "CoMemoryPool.h"

//Data structs
struct AsyncBlock {
    void (*executeFunction)();
    bool (*checkFunction)();
};

//Class
class CoAsync : CoMemoryPool {
public:
    //Constructor
    CoAsync(unsigned int functionAmount);

    //Functions
    bool add(void (*executeFunction)(), bool (*checkFunction)());
    void remove(void* address);
    void handle();
};

#endif