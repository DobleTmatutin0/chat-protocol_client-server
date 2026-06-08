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
#include <sys/select.h> // allows listening on multiple sockets



#include <ClientData.h>

// Server default direction.
#define IP "127.0.0.1" // this is LocalHost IP
#define PORT 8888

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1025

// VARIABLES

// Server's socket file descriptor
static int sv_sck_fd;

ClientData list_of_clients[MAX_CLIENTS];


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

    // puts the app to listen to connections
    int listen_result = listen(sv_sck_fd, MAX_CLIENTS);
    if (listen_result == -1) {
        perror("[SERVIDOR] (ERROR) listen()\n");
        exit(EXIT_FAILURE);
    }

    // initialize all client's sockets at 0
    for (int i = 0; i < MAX_CLIENTS; i++) {
        list_of_clients[i].socket_fd = 0;
    }

    // this is the mem space where are going to be saved all the file descriptos
    // that server should "listen"
    fd_set fds_to_listen;

    
    while(1) {
        // cleans the mem space variable
        FD_ZERO(&fds_to_listen);
        
        // add server's socket fd to the list to listen 
        FD_SET(sv_sck_fd, &fds_to_listen);
        
        // esto se usa para decir cual es el file descritor mas grande que escucha el sv
        int max_sck_fd_to_listen = sv_sck_fd;

        // add clint's socket fds to the list to listen
        for (int i = 0; i < MAX_CLIENTS; i++) {
            // saves temporaly the client's socket fd
            int clnt_sck_fd = list_of_clients[i].socket_fd;

            if (clnt_sck_fd > 0) {
                FD_SET(clnt_sck_fd, &fds_to_listen);
            }
            // updates max file descriptor to select
            if (clnt_sck_fd > max_sck_fd_to_listen) {
                max_sck_fd_to_listen = clnt_sck_fd;
            }
        }

        // select recorre todos los sockets en busqueda de actividad
        int activity = select(max_sck_fd_to_listen + 1, &fds_to_listen, NULL, NULL, NULL);
        if (activity == -1) {
            perror("[SERVIDOR] (ERROR) select()\n");
            exit(EXIT_FAILURE);
        }

        // si el socket del sv tuvo actividad entonces se procede sobre la misma
        if (FD_ISSET(sv_sck_fd, &fds_to_listen)) {
            socklen_t address_length = sizeof(address);

            // accepta conecciones
            int new_client_socket = accept(sv_sck_fd, (struct sockaddr*) &address, &address_length);
            if (new_client_socket == -1) {
                perror("[SERVIDOR] (ERROR) accept()\n");
                exit(EXIT_FAILURE);
            }
            
            printf(
                "Nueva conexión, socket fd: %d, IP: %s, Puerto: %d\n", 
                new_client_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port)
            );

            // save the new client socket
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (list_of_clients[1].socket_fd == 0) {
                    list_of_clients[1].socket_fd = new_client_socket;
                    break;
                }
            }
        }

        // continuar desde aca.
    }
}


