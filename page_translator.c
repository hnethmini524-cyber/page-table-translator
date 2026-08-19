#include <stdio.h>
#include <stdbool.h>

#define PAGE_SIZE 1024       // 1 KB = 1024 bytes
#define MAX_PAGES 16         // Logical address space limit (16 pages)
#define TOTAL_FRAMES 8       // Number of physical frames (8 to 16 allowed)
#define MAX_ADDRESSES 12     // Maximum addresses to process per run
#define MAX_BOUND (MAX_PAGES * PAGE_SIZE) // 16384 bytes

// Page table entry structure
typedef struct {
    int frame_num; 
    bool valid;    // true = loaded in memory, false = page fault
} PageTableEntry;

int main() {
    PageTableEntry page_table[MAX_PAGES];

    // Initialize Page Table with predefined sample values
    for (int i = 0; i < MAX_PAGES; i++) {
        page_table[i].valid = false;
        page_table[i].frame_num = -1;
    }

    // Predefining mapping for sample pages (0 to 15)
    page_table[0] = (PageTableEntry){.frame_num = 5, .valid = true};
    page_table[1] = (PageTableEntry){.frame_num = 2, .valid = true};
    page_table[2] = (PageTableEntry){.frame_num = -1, .valid = false};
    page_table[3] = (PageTableEntry){.frame_num = 7, .valid = true};
    page_table[5] = (PageTableEntry){.frame_num = 0, .valid = true};
    page_table[7] = (PageTableEntry){.frame_num = -1, .valid = false}; // -1 will trigger page fault
    page_table[8] = (PageTableEntry){.frame_num = 3,.valid = true};
    page_table[9] = (PageTableEntry){.frame_num = 4,.valid = true};
    page_table[10]= (PageTableEntry){.frame_num =-1,.valid = false};
    page_table[11]= (PageTableEntry){.frame_num = 2,.valid = true};
    page_table[12]= (PageTableEntry){.frame_num = -1,.valid = false};
    page_table[13]= (PageTableEntry){.frame_num = 1,.valid = true};
    page_table[14]= (PageTableEntry){.frame_num = 5,.valid = true};
    page_table[15]= (PageTableEntry){.frame_num = 7,.valid = true};

    printf("================== GROUP A - Page Table Translator ================== \n");
    printf("Page Size: %d Bytes | Max Pages: %d | Max Logical Memory: %d Bytes\n", PAGE_SIZE, MAX_PAGES, MAX_BOUND);
    printf("Physical Memory Frames Available: %d\n\n", TOTAL_FRAMES);

    // Page table mapping output
    printf("\nPage Table Mapping\n");
    printf("------------------------\n");
    printf("Page No\tFrame No\n");

    for (int i = 0; i < MAX_PAGES; i++) {
        if (page_table[i].valid) {
            printf("%d\t%d\n", i, page_table[i].frame_num);
        } else {
            printf("%d\t-\n", i);
        }
    }

    printf("------------------------\n");

    int num_inputs;
    printf("Enter number of logical addresses to process (Max %d): ", MAX_ADDRESSES);
    if (scanf("%d", &num_inputs) != 1 || num_inputs <= 0 || num_inputs > MAX_ADDRESSES) {
        printf("Invalid input number. Program exiting.\n");
        return 1;
    }

    long logical_addresses[MAX_ADDRESSES];
    printf("Enter %d logical addresses (in bytes):\n", num_inputs);
    for (int i = 0; i < num_inputs; i++) {
        printf(" Address %d: ", i + 1);
        scanf("%ld", &logical_addresses[i]);
    }

    // Output header
    printf("\n-----------------------------------------------------------------------------------------\n");
    printf("%-15s %-12s %-10s %-12s %-18s %-20s\n", 
           "Logical Addr", "Page No", "Offset", "Frame No", "Physical Addr", "Status / Remarks");

    // Process each logical address
    for (int i = 0; i < num_inputs; i++) {
        long address = logical_addresses[i];

        // Bounds check: must be < MAX_BOUND
        if (address < 0 || address >= MAX_BOUND) {
            printf("%-15ld %-12s %-10s %-12s %-18s %-20s\n", 
                   address, "N/A", "N/A", "N/A", "N/A", "Invalid / Out of Bounds");
            continue;
        }

        // Calculate page number and offset
        int page_num = address / PAGE_SIZE;
        int offset = address % PAGE_SIZE;

        // Translation check
        if (!page_table[page_num].valid) {
            printf("%-15ld %-12d %-10d %-12s %-18s %-20s\n", 
                   address, page_num, offset, "N/A", "N/A", "Page Fault Occurred");
        } else {
            int frame_num = page_table[page_num].frame_num;
            long physical_addr = (frame_num * PAGE_SIZE) + offset;
            printf("%-15ld %-12d %-10d %-12d %-18ld %-20s\n", 
                   address, page_num, offset, frame_num, physical_addr, "Successfully Translated");
        }
    }

    printf("-----------------------------------------------------------------------------------------\n");
    return 0;
}