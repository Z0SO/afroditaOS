#include "print/printf/printf.h"
#include <stdint.h>
#include "bootloader/bootservices.h"
#include "util/string.h"
#include "memory/memory.h"


// // funcion print toma puntero
// void print(const char* str) {

//     // en esta llamada voy a hacer un puntero que recibe una direccion de memoria a una funcion
//     // donde writer es un puntero que va a contener la direccion de memoria de get_terminal_writer()
//     void (*writer)(const char*, uint64_t) = get_terminal_writer();
//     writer(str, strlen(str));
// }

void hlt()
{
    while (1)
    {
        __asm__("hlt");
    }
}

void _start()
{

    init_memory();

    printf("sxlenOS\n bootloader -> %s\n version = %s\n",
        get_bootloader_name(),
        get_bootloader_version()
    );

    printf("Kernel looping...\n");

    hlt();
}