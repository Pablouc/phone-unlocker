#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <libwebsockets.h>

#define MAXBUF 1024
#define EXAMPLE_RX_BUFFER_SIZE 2048

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

/* Función para enviar el mensaje al servidor */
void send_message(int sockfd, char* buffer, struct sockaddr_in dest) {
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&dest, sizeof(dest));
}

// Función de callback para manejar eventos de WebSocket
static int websocket_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
    int sockfd;
    struct sockaddr_in dest;
    char buffer[MAXBUF];

    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("Conexión establecida\n");
            break;

        case LWS_CALLBACK_RECEIVE:
            printf("Mensaje recibido: %.*s\n", (int)len, (char*)in);

            // Aquí iniciamos la comunicación UDP
            sockfd = create_socket();
            dest = configure_server();
            strncpy(buffer, (char*)in, len);
            buffer[len] = '\0';
            send_message(sockfd, buffer, dest);
            memset(buffer, 0, sizeof(buffer));
            close(sockfd);
            break;

        default:
            break;
    }

    return 0;
}

// Configuración del protocolo WebSocket
static struct lws_protocols protocols[] = {
    {
        "websocket",
        websocket_callback,
        0,
        EXAMPLE_RX_BUFFER_SIZE,
    },
    { NULL, NULL, 0, 0 } // Marca el final de la lista de protocolos
};

int main(void) {
    struct lws_context* context;
    struct lws_context_creation_info info;

    memset(&info, 0, sizeof(info));
    info.port = 8080; // Puerto por defecto en el que escucha el servidor WebSocket
    info.iface = "wlp2s0"; // Interfaz de red en la que escucha el servidor WebSocket
    info.protocols = protocols;

    // Crea el contexto del servidor
    context = lws_create_context(&info);
    if (!context) {
        printf("Error al crear el contexto del servidor WebSocket\n");
        return -1;
    }

    // Ejecuta el ciclo principal del servidor WebSocket
    while (1) {
        lws_service(context, 50); // Espera eventos durante 50 ms
    }

    lws_context_destroy(context);

    return 0;
}
