#ifndef TLB_H
#define TLB_H

typedef struct {
    int page;
    int frame;
    int valid;
} tlb_entry_t;

void tlb_init(void);

int tlb_lookup(int page);

void tlb_insert(int page, int frame);

void tlb_remove(int page);

void tlb_clear(void);

#endif
