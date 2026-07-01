#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

typedef struct {
    int frame;
    int valid;

    /*
     * Campos usados no LRU aproximado.
     */
    unsigned char reference_bit;
    unsigned char aging_counter;
} page_table_entry_t;

void page_table_init(void);

int page_table_lookup(int page);

void page_table_update(int page, int frame);

void page_table_invalidate(int page);

void page_table_set_reference(int page);

void page_table_update_aging(void);

int page_table_get_frame(int page);

int page_table_is_valid(int page);

unsigned char page_table_get_aging_counter(int page);

#endif
