

#ifndef MEMALLOC_MEMORYALLOCATOR_H
#define MEMALLOC_MEMORYALLOCATOR_H

#include  "../lib/hw.h"
#include "fragment_desc.hpp"

class MemoryAllocator {
public:
    static MemoryAllocator* getInstance();
    void* kmalloc(size_t size);

    int memfree(void *ptr);
private:
    MemoryAllocator();
    void init(const void *start, const void *end);
    ~MemoryAllocator();
    MemoryAllocator(const void *start, const void *end);

    fragment_desc* head;
    static MemoryAllocator* instance;
    static bool initialised;
};







#endif //MEMALLOC_MEMORYALLOCATOR_H