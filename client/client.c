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
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("127.0.0.1"); // servidor en localhost
    dest.sin_port = htons(1234); // puerto del servidor
    return dest;
}

/* Función para obtener y cifrar el mensaje */
void get_and_encrypt_message(char* buffer) {
    printf("Ingrese un mensaje para enviar al servidor:\n");
    fgets(buffer, MAXBUF, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // quitar el newline
    // Aquí es donde debes implementar la cifrado del mensaje.
}

/* Función para enviar el mensaje al servidor */
void send_message(int sockfd, char* buffer, struct sockaddr_in dest) {
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&dest, sizeof(dest));
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in dest;
    char buffer[MAXBUF];

    sockfd = create_socket();
    dest = configure_server();
    get_and_encrypt_message(buffer);
    send_message(sockfd, buffer, dest);

    /* Limpiar y cerrar */
    memset(buffer, 0, sizeof(buffer));
    close(sockfd);

    return 0;
}
