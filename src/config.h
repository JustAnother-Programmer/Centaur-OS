#ifndef CONFIG_H
#define CONFIG_H

#define CENTAUROS_TOTAL_INTR 512
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define CENTAUROS_HEAP_SIZE_BYTES 104857600 // 100MB heap
#define CENTAUROS_HEAP_BLOCK_SIZE 4096
#define CENTAUROS_HEAP_ADDRESS 0x01000000
#define CENTAUROS_HEAP_TABLE_ADDRESS 0x00007E00

#define CENTAUROS_SECTOR_SIZE 512

#define CENTAUROS_MAX_FILESYSTEMS 12
#define CENTAUROS_MAX_FILE_DESCRIPTORS 512
#define CENTAUROS_MAX_PATH 108

#define CENTAUROS_TOTAL_GDT_SEGMENTS 6

#endif