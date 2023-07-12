#include "bootservices.h"
#define STRING_MAX_SIZE 65536

uint64_t strlen(const char* str){
    uint64_t len=0;
    while (str[len] && len < STRING_MAX_SIZE)
        len++;
    return len;
}


// defino la funcion print que le voy a pasar un puntero
void print(const char* str) {

    // en esta llamada voy a hacer un puntero que recibe una direccion de memoria a una funcion
    // donde writer es un puntero que va a contener la direccion de memoria de get_terminal_writer()
    void (*writer)(const char*, uint64_t) = get_terminal_writer();
    writer(str, strlen(str));
}

void hlt(){
    while (1){
        __asm__("hlt");
    }
}


void _start(){
    print("hello world!");
    hlt();
}