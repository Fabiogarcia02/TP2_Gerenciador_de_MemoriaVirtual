#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "tlb.h"
#include "page_table.h"
#include "memory.h"
#include "statistics.h"

int main(void)
{
    FILE *backing = fopen(BACKING_STORE_PATH, "rb");

    if (backing == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir %s\n", BACKING_STORE_PATH);
        fprintf(stderr, "Execute antes: cd data && python3 generate_data.py\n");
        return 1;
    }

    page_table_init();
    tlb_init();
    memory_init(backing);
    statistics_init();

    int logical_address;

    while (scanf("%d", &logical_address) == 1) {
        count_address();

        /*
         * O arquivo de entrada possui inteiros de 32 bits.
         * Apenas os 16 bits menos significativos devem ser considerados.
         * - Mascara-se 0xFFFF para isolar o endereço lógico de 16 bits.
         * - Os 8 bits superiores formam o número da página.
         * - Os 8 bits inferiores formam o deslocamento (offset).
         */
        int masked_address = logical_address & 0xFFFF;
        int page = (masked_address >> 8) & 0xFF;
        int offset = masked_address & 0xFF;

        int frame = tlb_lookup(page);

        if (frame != -1) {
            count_tlb_hit();
        } else {
            frame = page_table_lookup(page);

            if (frame == -1) {
                count_page_fault();
                frame = handle_page_fault(page);
            }

            tlb_insert(page, frame);
        }

        /*
         * TODO:
         * A política de atualização do LRU aproximado pode ser ajustada.
         * Nesta versão-base, a página acessada é marcada como referenciada
         * e o aging é atualizado a cada acesso.
         */

        page_table_set_reference(page);
        page_table_update_aging();
        
        /*
         * Endereço físico = quadro * tamanho_do_quadro + offset.
         */
        int physical_address = frame * FRAME_SIZE + offset;
        signed char value = read_memory(frame, offset);

        printf("Logical address: %d Physical address: %d Value: %d\n",
               logical_address,
               physical_address,
               value);
    }

    print_statistics();

    fclose(backing);

    return 0;
}
