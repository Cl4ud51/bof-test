#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9999
#define BUFFER_SIZE 128 // 固定長バッファ

void handle_client(int client_sock) {
    char user_input[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    
    send(client_sock, "Welcome to the vulnerable server!\n", 35, 0);
    send(client_sock, "USER: ", 7, 0);
    recv(client_sock, user_input, BUFFER_SIZE, 0);
    send(client_sock, "PASS: ", 7, 0);
    recv(client_sock, password, 512, 0); // バッファオーバーフローの脆弱性
    
    char vulnerable_buffer[BUFFER_SIZE];
    strcpy(vulnerable_buffer, password); // オーバーフローが発生する可能性
    
    if (strncmp(user_input, "QUIT", 4) == 0 || strncmp(user_input, "EXIT", 4) == 0) {
        send(client_sock, "Goodbye!\n", 9, 0);
        close(client_sock);
        return;
    }
    
    send(client_sock, "Login successful!\n", 18, 0);
    printf("User logged in.\n");
    
    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("[-] Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Bind error");
        exit(1);
    }

    if (listen(server_sock, 5) < 0) {
        perror("[-] Listen error");
        exit(1);
    }

    printf("[+] Listening on port %d...\n", PORT);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_size);
        if (client_sock < 0) {
            perror("[-] Accept error");
            continue;
        }
        printf("[+] Connection accepted.\n");
        handle_client(client_sock);
    }

    close(server_sock);
    return 0;
}

