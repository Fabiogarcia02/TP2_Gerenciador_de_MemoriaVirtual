#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

void memory_init(FILE *backing_store);

int handle_page_fault(int page);

int select_victim_page(void);

signed char read_memory(int frame, int offset);

int get_page_loaded_in_frame(int frame);

#endif
