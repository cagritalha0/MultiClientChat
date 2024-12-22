#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesi

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Winsock'u başlat
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // İstemci soketi oluştur
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Sunucuya bağlan
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection to server failed. Error Code: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Mesajı sunucuya gönder
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            printf("Send failed. Error Code: %d\n", WSAGetLastError());
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);

        // Sunucudan yanıt al
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) < 0) {
            printf("Receive failed. Error Code: %d\n", WSAGetLastError());
            break;
        }

        printf("Server: %s\n", buffer);
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
