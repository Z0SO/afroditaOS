#include "memory.h"
#include "../bootloader/bootservices.h"
#include "../print/printf/printf.h"

struct uefi_memory_region working_memory = {
    .type = LIMINE_MEMMAP_BAD_MEMORY,
    .length = 0,
    .base = 0
};

void init_memory()
{
    printf("Cargando memoria...\n");

    uint64_t entries = get_memory_map_entries();

    printf("DETECTADAS %d regiones de memoria\n", entries);

    uint64_t total_memory = 0;
    uint64_t total_avail_memory = 0;    //sera el tipo de memoria de memmap usable

    for (uint64_t i = 0; i < entries; i++){

        uint64_t type = get_memory_map_type(i);
        uint64_t length = get_memory_map_length(i);
        uint64_t base = get_memory_map_base(i);

        printf("\nMemory region %d base= %x length=%x type=%d\n",
            i,
            base,
            length,
            type
        );

        total_memory += length;

        if (type == LIMINE_MEMMAP_USABLE){
            total_avail_memory+=length;

            if (length > working_memory.length){
                working_memory.length = length;
                working_memory.type = type;
                working_memory.base = base;
            }
        }
    };
    
    printf("Total de memoria: %x, memoria disponible: %x, .\n",
        total_memory,
        total_avail_memory
    );


    printf("memoria usada: [base: %x. type: %x. size: %x.]\n",
        working_memory.base,
        working_memory.type,
        working_memory.length
    );
};