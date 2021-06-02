#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

void* kmalloc(size_t size);

void kfree(void* ptr);

void kheap_init();

#endif