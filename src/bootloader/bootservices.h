#ifndef _BOOTSERVICES_H

#define _BOOTSERVICES_H

#include "limine.h"
#include <stdint.h>

//void   (*get_terminal_writer())(const char*, uint64_t);


// se hacen un par de funciones para obtener cada uno de los elementos de limine_request

char * get_bootloader_name();       // para obtener el nombre del bootloader
char * get_bootloader_version();    // para obtener la version del bootloader

uint64_t get_terminal_count();
uint64_t get_current_version();

// parte de memoria fisica
uint64_t get_memory_map_entries();
uint64_t get_memory_map_base(uint64_t entry);
uint64_t get_memory_map_length(uint64_t entry);
uint64_t get_memory_map_type(uint64_t entry);


// alternativa para la memoria fisica
// void load_memory_descriptor(struct uefi_memory_region* region);



// identificar cual de las terminales utilizar
void set_terminal_writer(uint64_t terminal);


// funcion que retornara la terminal que permita escribirse en ella
void generic_terminal_writer(const char* str, uint64_t length);

// crear una funcion que retorne un puntero a la funcion generic_terminal_writer del bootservices.c
void (*get_terminal_writer())(const char*, uint64_t);


#endif