#include "CoMemoryPool.h"

#include <stdlib.h>

//
// Constructor
//
CoMemoryPool::CoMemoryPool(unsigned int size, int blockAmount) : 
        memorySize(size + sizeof(MemoryBlock) * blockAmount),
        blockAmount(blockAmount),
        blockSize(size / blockAmount) {
    memoryPool = ::malloc(memorySize);
    assignMemoryBlocks();
}

//
// Deconstructor
//
CoMemoryPool::~CoMemoryPool() {
    ::free(memoryPool);
}

//
// Assign empty memory blocks
//
void CoMemoryPool::assignMemoryBlocks() {
    MemoryBlock* prevBlock = nullptr;
    //Loop over the block amount and make blocks
    for(int i = 0; i < blockAmount; i++) {
        MemoryBlock* newBlock = (MemoryBlock*)((char*)memoryPool + i * (blockSize + sizeof(MemoryBlock)));
        newBlock->next = nullptr;
        newBlock->prev = prevBlock;
        if(i > 0) {
            prevBlock->next = newBlock;
        }
        prevBlock = newBlock;
    }
    //Let the freeMemoryBlocks point to the first byte
    freeMemoryBlocks = (MemoryBlock*)memoryPool;
}

//
// Allocate data
//
AllocResult CoMemoryPool::alloc(unsigned int size) {
    AllocResult result;
    result.success = false;
    result.address = nullptr;
    //Check if the requested size is large enough
    if(size > blockSize) {
        return result;
    }
    //Check if there is still an block left
    if(freeMemoryBlocks == nullptr) {
        return result;
    }
    //Get an block
    MemoryBlock* memoryBlock = freeMemoryBlocks;
    //Set the first block to be used as the next block
    freeMemoryBlocks = memoryBlock->next;
    //Remove this block from the list of freeBlocks
    if(memoryBlock->prev != nullptr) {
        memoryBlock->prev->next = memoryBlock->next;
    }
    if(memoryBlock->next != nullptr) {
        memoryBlock->next->prev = memoryBlock->prev;
    }
    //Add the block to the beginning of the usedMemoryBlock array
    memoryBlock->next = usedMemoryBlocks;
    memoryBlock->prev = nullptr;
    if(usedMemoryBlocks != nullptr) {
        usedMemoryBlocks->prev = memoryBlock;
    }
    usedMemoryBlocks = memoryBlock;
    result.address = (void*)((char*)memoryBlock + sizeof(MemoryBlock));
    result.success = true;
    return result;
}

//
// Deallocate data
//
bool CoMemoryPool::free(void* address) {
    MemoryBlock* block = (MemoryBlock*)((char*)address - sizeof(MemoryBlock));
    //Check if the address is inside the block
    if((int*)address + blockSize > (int*)memoryPool && (int*)address + blockSize < (int*)memoryPool + memorySize) {
        //Empty everything in the block
        for(int i = 0; i < blockSize; i++) {
            *((char*)address + i) = 0;
        }
        //Unlink the block from the list
        if(block->prev != nullptr) {
            block->prev->next = block->next;
        }
        if(block->next != nullptr) {
            block->next->prev = block->prev;
        }
        //If there is no previous block (starting point). Point to the next item from this block.
        if(block->prev == nullptr) {
            usedMemoryBlocks = block->next;
        }
        //Remove all references
        block->next = nullptr;
        block->prev = nullptr;
        //Add the item to the start of the list
        if(freeMemoryBlocks != nullptr) {
            freeMemoryBlocks->prev = block;
        }
        block->next = freeMemoryBlocks;
        //Set the free memoryBlock to this block
        freeMemoryBlocks = block;
        return true;
    } else {
        return false;
    }
}