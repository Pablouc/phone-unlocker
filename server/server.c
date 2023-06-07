#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXBUF 1024

/* Función para crear el socket */
int create_socket() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

/* Función para configurar la estructura del servidor */
struct sockaddr_in configure_server() {
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // cualquier interfaz
    servaddr.sin_port = htons(1234); // puerto del servidor
    return servaddr;
}

/* Función para vincular el socket */
void bind_socket(int sockfd, struct sockaddr_in servaddr) {
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error al vincular el socket");
        exit(EXIT_FAILURE);
    }
}

/* Función para descifrar el mensaje */
void decrypt_message(char* buffer, int buffer_length) {
    int shift = 6;  // Cantidad de desplazamiento para el cifrado César.

    /* Descifrado César */
    for (int i = 0; i < buffer_length; ++i) {
        char c = buffer[i];
        if ('a' <= c && c <= 'z') {
            buffer[i] = 'a' + (c - 'a' - shift + 26) % 26;
        } else if ('A' <= c && c <= 'Z') {
            buffer[i] = 'A' + (c - 'A' - shift + 26) % 26;
        } else if ('0' <= c && c <= '9') {
            buffer[i] = '0' + (c - '0' - shift + 10) % 10;
        }
    }
}

/* Función para recibir el mensaje del cliente */
int receive_message(int sockfd, char* buffer, struct sockaddr_in cliaddr, socklen_t addr_len) {
    int n = recvfrom(sockfd, buffer, MAXBUF, 0, (struct sockaddr *)&cliaddr, &addr_len);
    if (n < 0) {
        perror("Error al recibir el mensaje");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';

    char buffer_copy[MAXBUF];
    strcpy(buffer_copy, buffer);

    printf("Mensaje recibido (cifrado): %s\n", buffer_copy);

    decrypt_message(buffer, n);

    return n;
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXBUF];
    socklen_t addr_len;

    sockfd = create_socket();
    servaddr = configure_server();
    bind_socket(sockfd, servaddr);

    printf("Esperando cliente... \n");
    while (1) {
        addr_len = sizeof(cliaddr);

        int n = receive_message(sockfd, buffer, cliaddr, addr_len);

        printf("Mensaje recibido (descifrado): %s\n", buffer);

        /* Limpiar el buffer */
        memset(buffer, 0, sizeof(buffer));

        printf("Esperando cliente... \n");
    }

    /* Cerrar el socket */
    close(sockfd);

    return 0;
}
