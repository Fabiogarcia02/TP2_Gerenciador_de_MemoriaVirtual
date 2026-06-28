#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "config.h"
#include "page_table.h"
#include "tlb.h"

static signed char physical_memory[NUM_FRAMES][FRAME_SIZE];

/*
 * Indica qual página está carregada em cada quadro.
 * Valor -1 indica quadro livre.
 */
static int frame_to_page[NUM_FRAMES];

static FILE *backing = NULL;

void memory_init(FILE *backing_store)
{
    backing = backing_store;

    for (int i = 0; i < NUM_FRAMES; i++) {
        frame_to_page[i] = -1;

        for (int j = 0; j < FRAME_SIZE; j++) {
            physical_memory[i][j] = 0;
        }
    }
}

static int find_free_frame(void)
{
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frame_to_page[i] == -1) {
            return i;
        }
    }

    return -1;
}

int handle_page_fault(int page)
{
    if (backing == NULL) {
        fprintf(stderr, "Erro interno: BACKING_STORE nao inicializado.\n");
        exit(1);
    }

    /* 1. Procura um quadro livre. */
    int frame = find_free_frame();

    /* 2. Sem quadro livre: aplica substituição (LRU aproximado). */
    if (frame == -1) {
        int victim_page = select_victim_page();
        frame = page_table_get_frame(victim_page);

        /* 3 e 4. Invalida a vítima na tabela de páginas e no TLB. */
        page_table_invalidate(victim_page);
        tlb_remove(victim_page);

        frame_to_page[frame] = -1;
    }

    /* 5. Lê a página solicitada do BACKING_STORE.bin (acesso aleatório). */
    if (fseek(backing, (long) page * PAGE_SIZE, SEEK_SET) != 0) {
        fprintf(stderr, "Erro: falha no fseek para a pagina %d.\n", page);
        exit(1);
    }

    if (fread(physical_memory[frame], sizeof(signed char), PAGE_SIZE, backing)
        != PAGE_SIZE) {
        fprintf(stderr, "Erro: falha na leitura da pagina %d.\n", page);
        exit(1);
    }

    /* 6 e 7. Atualiza o mapeamento quadro->página e a tabela de páginas. */
    frame_to_page[frame] = page;
    page_table_update(page, frame);

    /* 8. Retorna o quadro onde a página foi carregada. */
    return frame;
}

int select_victim_page(void)
{
    /*
     * Escolhe a página residente com menor aging_counter.
     * Em caso de empate, mantém a primeira encontrada (menor índice de quadro).
     */
    int victim_page = -1;
    unsigned char min_counter = 0;

    for (int i = 0; i < NUM_FRAMES; i++) {
        int page = frame_to_page[i];

        if (page == -1) {
            continue;
        }

        unsigned char counter = page_table_get_aging_counter(page);

        if (victim_page == -1 || counter < min_counter) {
            min_counter = counter;
            victim_page = page;
        }
    }

    return victim_page;
}

signed char read_memory(int frame, int offset)
{
    if (frame < 0 || frame >= NUM_FRAMES ||
        offset < 0 || offset >= FRAME_SIZE) {
        return 0;
    }

    return physical_memory[frame][offset];
}

int get_page_loaded_in_frame(int frame)
{
    if (frame < 0 || frame >= NUM_FRAMES) {
        return -1;
    }

    return frame_to_page[frame];
}
