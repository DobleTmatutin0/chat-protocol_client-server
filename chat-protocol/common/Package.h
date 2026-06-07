#include <stdint.h>

#include <OpCode.h>


// Resolver el tema de numeros magicos

typedef struct {
    OpCode opcode;
    uint16_t length;
    char to[32];
    char data[512];
} Package;