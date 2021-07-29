#include "task.h"
#include "status.h"
#include "kernel.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"

struct task* current_task = 0;
struct task* task_tail = 0;
struct task* task_head = 0;

int task_init(struct task* task);

struct task* task_current()
{
    return current_task;
}

struct task* task_new()
{
    int res = 0;

    struct task* task = kzalloc(sizeof(struct task));

    if(!task)
    {
        res = -ENOMEM;
        goto out;
    }

    res = task_init(task);

    if(res != CENTAUROS_ALL_OK)
    {
        goto out;
    }

    if(task_head == 0)
    {
        task_head = task;
        task_tail = task;
        goto out;
    }

    task_tail->next = task;
    task->prev = task_tail;
    task_tail = task;

out:
    if(ISERR(res))
    {
        task_free(task);
        return ERROR(res);
    }

    return task;
}

struct task* task_get_next()
{
    if(!current_task->next)
    {
        return task_head;
    }

    return current_task->next;
}

static void task_list_remove(struct task* task)
{
    if(task->prev)
    {
        task->prev->next = task->next;
    }

    if(task == task_head)
    {
        task_head = task->next;
    }

    if(task == task_tail)
    {
        task_tail = task->prev;
    }

    if(task == current_task)
    {
        current_task = task_get_next();
    }
}

int task_free(struct task* task)
{
    paging_free_4gb(task->page_directory);
    task_list_remove(task);

    kfree(task);
    return 0;
}

int task_init(struct task* task)
{
    memset(task, 0 , sizeof(struct task));
    task->page_directory = paging_new_4gb(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    if(!task->page_directory)
    {
        return -EIO;
    }

    task->registers.ip = CENTAUROS_PROGRAM_VIRTUAL_ADDRESS;
    task->registers.ss = USER_DATA_SEGMENT;
    task->registers.esp = CENTAUROS_PROGRAM_VIRTUAL_STACK_ADDRESS_START;

    return 0;
}