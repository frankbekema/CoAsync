#include "CoAsync.h"

//
// Constructor
//
CoAsync::CoAsync(unsigned int functionAmount) : CoMemoryPool(functionAmount * sizeof(AsyncBlock), functionAmount) { }

//
// Add a function
//
bool CoAsync::add(void (*executeFunction)(), bool (*checkFunction)()) {
    //Allocate memory
    AllocResult res = alloc(sizeof(AsyncBlock));
    //Check if the memory was allocated
    if(res.success) {
        //Allocate the async function block
        AsyncBlock asyncBlock;
        asyncBlock.executeFunction = executeFunction;
        asyncBlock.checkFunction = checkFunction;
        *((AsyncBlock*)((void*)res.address)) = asyncBlock;
        return true;
    } else {
        return false;
    }
}

//
// Remove a function 
//
void CoAsync::remove(void* address) {
    this->free(address);
}

//
// Function that checks if an Async function can be executed;
//
void CoAsync::handle() {
    bool blockRemoved = false;
    MemoryBlock* block = usedMemoryBlocks;
    //Loop over all blocks
    while(block != nullptr) {
        //Fetch AsyncBlock
        AsyncBlock* asyncBlock = ((AsyncBlock*)((char*)block + sizeof(MemoryBlock)));
        blockRemoved = asyncBlock->checkFunction();
        //If true, execute the function and remove the block
        if(blockRemoved) {
            asyncBlock->executeFunction();
            this->remove((void*)asyncBlock);
            break;
        }
        block = block->next;
    }
    //Re-Start handler if an block got removed
    if(blockRemoved) {
        this->handle();
    }
}