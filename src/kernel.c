#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "config.h"
#include "idt/idt.h"
#include "task/tss.h"
#include "io/io.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "task/task.h"
#include "task/process.h"
#include "disk/disk.h"
#include "gdt/gdt.h"
#include "status.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "fs/file.h"
#include "disk/streamer.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour)
{
    if(c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    
    if(terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_init()
{
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;

    for(int y = 0; y < VGA_HEIGHT; y++)
    {
        for(int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

void print(const char* str)
{
    size_t len = strlen(str);

    for(int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

static struct paging_4gb_chunk* kernel_chunk = 0;

void panic(const char* msg)
{
    print(msg);
    while(1) {}
}

struct tss tss;

struct gdt gdt_real[CENTAUROS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[CENTAUROS_TOTAL_GDT_SEGMENTS] = {
    {.base = 0x00, .limit = 0x00, .type = 0x00}, // Null seg
    {.base = 0x00, .limit = 0xffffffff, .type = 0x9a}, // Kernel code seg
    {.base = 0x00, .limit = 0xffffffff, .type = 0x92}, // Kernel data seg
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf8}, // User code seg
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf2}, // User data seg
    {.base = (uint32_t)&tss, .limit = sizeof(tss), .type = 0xE9} // TSS Seg
};

void kernel_main()
{
    terminal_init();
    
    print("Welcome to Centaur OS! Loading System...\n");

    memset(gdt_real, 0x00, sizeof(gdt_real));

    gdt_structured_to_gdt(gdt_real, gdt_structured, CENTAUROS_TOTAL_GDT_SEGMENTS);

    gdt_load(gdt_real, sizeof(gdt_real));

    kheap_init();

    fs_init();

    disk_search_and_init();

    idt_init();

    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;

    tss_load(0x28);

    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    enable_paging();

    struct process* process = 0;

    int res = process_load("0:/blank.bin", &process);

    if(res != CENTAUROS_ALL_OK)
    {
        panic("Failed to load blank.bin\n");
    }

    task_run_first_ever_task();

    print("Loading complete.\n");

    while(1) {}
}