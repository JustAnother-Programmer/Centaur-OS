#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

void* kmalloc(size_t size);

void kfree(void* ptr);

void* kzalloc(size_t size);

void kheap_init();

#endif