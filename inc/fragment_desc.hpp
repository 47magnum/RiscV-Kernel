//
// Created by Mladen Milicevic on 8. 6. 2026..
//



#ifndef MEMALLOC_FRAGMENT_DESC_H
#define MEMALLOC_FRAGMENT_DESC_H
struct fragment_desc {
    uint64 size; //Broj slobodnih BLOCK SIZE-ova
    fragment_desc* next;
};


#endif //MEMALLOC_REGIONDESC_H