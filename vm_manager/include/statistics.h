#ifndef STATISTICS_H
#define STATISTICS_H

void statistics_init(void);

void count_address(void);

void count_page_fault(void);

void count_tlb_hit(void);

int get_total_addresses(void);

int get_page_faults(void);

int get_tlb_hits(void);

void print_statistics(void);

#endif
