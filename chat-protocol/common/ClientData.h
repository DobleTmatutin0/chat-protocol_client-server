#include <stdbool.h>

#define MAX_USER_NAME_LENGTH 32


typedef struct {
    int socket_fd;                     
    char username[MAX_USER_NAME_LENGTH];    
    bool logged_in; // Si está logueado es 1, sino 0
} ClientData;