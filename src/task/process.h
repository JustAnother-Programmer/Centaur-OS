#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "config.h"
#include "task.h"

struct process
{
    uint16_t id;

    char filename[CENTAUROS_MAX_PATH];

    struct task* task;

    void* allocations[CENTAUROS_MAX_PROGRAM_ALLOCATIONS];

    void* ptr;

    void* stack;

    uint32_t size;
};

int process_load_for_slot(const char* filename, struct process** process, int process_slot);

#endif