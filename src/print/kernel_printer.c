#include "kernel_printer.h"
#include "../bootloader/bootservices.h"


void _putchar(char character){

    // en esta llamada voy a hacer un puntero que recibe una direccion de memoria a una funcion
    // donde writer es un puntero que va a contener la direccion de memoria de get_terminal_writer()
    void (*writer)(const char*, uint64_t) = get_terminal_writer();
    writer(&character, 1);
}