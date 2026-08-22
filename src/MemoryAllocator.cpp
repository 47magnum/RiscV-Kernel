
#include "../inc/MemoryAllocator.hpp"

#include "../inc/fragment_desc.hpp"
#include  "../lib/hw.h"

 bool MemoryAllocator::initialised = false;
MemoryAllocator* MemoryAllocator::instance = nullptr;


void MemoryAllocator::init(const void* start, const void* end) {
    char* start_ptr = (char*)start;
    char* end_ptr = (char*)end;
    size_t total_bytes = end_ptr - start_ptr + 1;
    size_t total_blocks = total_bytes / MEM_BLOCK_SIZE;
    head = (fragment_desc*)start;
    head->size = total_blocks;
    head->next = nullptr;
}

MemoryAllocator::~MemoryAllocator(){

}



MemoryAllocator* MemoryAllocator::getInstance() {
    if (!initialised) {
        char* base = (char*)HEAP_START_ADDR + sizeof(MemoryAllocator);
        // Poravnaj na 16 bajta
        uint64 base_addr = (uint64)base;
        base_addr = (base_addr + 15) & ~15ULL;
        char* start_adr = (char*)base_addr;
        char* end_adr = (char*)HEAP_END_ADDR - 1;
        instance = (MemoryAllocator*)HEAP_START_ADDR;
        instance->init(start_adr, end_adr);
        initialised = true;
    }
    return instance;
}

void* MemoryAllocator::kmalloc(size_t size) {
    uint64 num_blocks = (size + sizeof(fragment_desc) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    if (!head) return nullptr;

    fragment_desc* curr = head;
    fragment_desc* prev = nullptr;

    while (curr) {
        if (curr->size >= num_blocks) break;
        prev = curr;
        curr = curr->next;
    }

    if (!curr) return nullptr;

    if (curr->size > num_blocks) {
        // Ostaje slobodan fragment iza alokacije
        fragment_desc* new_free = (fragment_desc*)((char*)curr + num_blocks * MEM_BLOCK_SIZE);
        new_free->size = curr->size - num_blocks;
        new_free->next = curr->next;

        if (prev) prev->next = new_free;
        else head = new_free;

        curr->size = num_blocks;
        return (char*)curr + sizeof(fragment_desc);
    } else {
        // Perfektno poklapanje
        if (prev) prev->next = curr->next;
        else head = curr->next;

        curr->size = num_blocks;
        return (char*)curr + sizeof(fragment_desc);
    }
}





int MemoryAllocator::memfree (void* ptr){
    if (ptr == nullptr)return  -1;
    char* test = (char*)ptr;
    if (test < HEAP_START_ADDR || test > HEAP_END_ADDR)return -1;
    char* descriptor = (char*)ptr - sizeof(fragment_desc);
    fragment_desc* frag_desc = (fragment_desc*)descriptor;

    fragment_desc* curr = MemoryAllocator::head;
    fragment_desc* prev = nullptr;
    while (curr && curr < frag_desc) {
        prev = curr;
        curr = curr->next;
    }
    //sad prev pokazuje na fragment pre a curr na fragment posle
    if (prev) {
        if (prev->size * MEM_BLOCK_SIZE + (char*)prev == (char*)frag_desc) {//proveriti aritmetiku
            //spoji sa proslim u jedan fragment
            prev->size += frag_desc->size;
            frag_desc = prev;
        }
        else {
            prev->next = frag_desc;
            frag_desc->next = curr;
        }
    }
    else {
       frag_desc->next = head;
        head = frag_desc;
    }
    if (curr) {
        if ((char*)frag_desc + frag_desc->size * MEM_BLOCK_SIZE == (char*)curr) {
            //spojeni fragmenti
            frag_desc->size += curr->size;
            frag_desc->next = curr->next;
        }
        else {
            frag_desc->next = curr;

        }

    }
    return 0;


}