// standard ANSI / ISO C library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// Librerias del estandar POSIX
#include <sys/socket.h> // Berkeley sockets
#include <arpa/inet.h> // functions for manipulate ip addresses (part of Berkeley sockets)
#include <netinet/ip.h> // defines InternetProtocol and addess family (part of Berkeley sockets)
#include <unistd.h> // syscalls
#include <sys/types.h> // defines various data types


// Server default direction.
#define IP "127.0.0.1" // this is LocalHost IP
#define PORT 8888

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1025

// VARIABLES

// Server's socket file descriptor
static int sv_sck_fd;


void signHandler(int signal) {
    close(sv_sck_fd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    // struct to save IP addresses (saves type (IPvX), IP dir and Port)
    struct sockaddr_in address;

    printf("[SERVIDOR] Iniciando Chat Server...\n");

    // open an IPv4 TCP socket
    sv_sck_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sv_sck_fd < 0) {
        perror("[SERVIDOR] (ERROR) socket()\n");
        exit(EXIT_FAILURE);
    }

    // clean 'ip_addess' variable
    // sets all the bytes of the memory space at 0
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET; // sets IP family to IPv4 family 
    // sets IP and PORT
    if (argc == 3) {
        // use argument IP and PORT
        inet_aton(argv[1], &(address.sin_addr));
        address.sin_port = htons((uint16_t) atoi(argv[2]));
    }
    else {
        // use default IP and PORT
        inet_aton(IP, &(address.sin_addr));
        address.sin_port = htons(PORT);
    }

    // VER
    // falta elcodigo para permitir reutilizar la direccion asociada al socker

    // Associates the socket with the selected address.
    int bind_result = bind(sv_sck_fd, (struct sockaddr*) &address, sizeof(address));
    if (bind_result == -1) {
        perror("[SERVIDOR] (ERROR) bind()\n");
        exit(EXIT_FAILURE);
    }

    // puts the app to listen for connections
    int listen_result = listen(sv_sck_fd, MAX_CLIENTS);
    if (listen_result == -1) {
        perror("[SERVIDOR] (ERROR) listen()\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        
    }
}


