#include <ps4.h>
#include "utils.h"

unsigned int get_host_ip(char* buff_ip) {
    // This will lock payload's execution until a connecton arrives
    // Then we know host's ip
    struct sockaddr_in server_addr = {
        .sin_addr = {
            .s_addr = sceNetHtonl(0)
        },
        .sin_family = AF_INET,
        .sin_port = sceNetHtons(4747)
    };
    struct sockaddr_in client_addr;
    
    int server_sock;
    int client_sock;

    server_sock = sceNetSocket("debug", AF_INET, SOCK_STREAM, 0);

    if(server_sock < 0) {
        sceSysUtilSendSystemNotificationWithText(222, "Can't boot TCP socket");
        return -1;
    }

    if(sceNetBind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        sceSysUtilSendSystemNotificationWithText(222, "Can't bind TCP socket");
        return -1;
    }

    if(sceNetListen(server_sock, 5) < 0) {
        sceSysUtilSendSystemNotificationWithText(222, "Can't listen TCP socket");
        return -1;
    }

    int client_addr_size = sizeof(client_addr);
    client_sock = sceNetAccept(server_sock, (struct sockaddr*)&client_addr, (unsigned int*)&client_addr_size);

    if(client_sock < 0) {
        sceSysUtilSendSystemNotificationWithText(222, "Can't accept TCP connection");
        return -1;
    } else {
        sceSysUtilSendSystemNotificationWithText(222, "Connection accepted");
    }

    sceNetSocketClose(client_sock);
    sceNetSocketClose(server_sock);



    unsigned int host_ip = client_addr.sin_addr.s_addr;
    char *buff = malloc(100);
    char *ip = malloc(20);

    sceNetInetNtop(AF_INET, &host_ip, ip, 20);
    snprintf(buff, 100, "Connection from: %s", ip);
    sceSysUtilSendSystemNotificationWithText(222, buff);
    strcpy(buff_ip, ip);

    free(buff);
    free(ip);

    return host_ip;
}

int request_get(char *ip, int port, char *path, char *buff, size_t buff_size) {
    struct in_addr ip_addr;
    struct sockaddr_in server_addr = {
        .sin_port = sceNetHtons(4848),
        .sin_family = AF_INET,
        .sin_addr = {
            .s_addr = IP(192, 168, 100, 50)
        }
    };
    sceNetInetPton(AF_INET, ip, &ip_addr);
    server_addr.sin_len = sizeof(server_addr);
    server_addr.sin_addr = ip_addr;
    int sock = sceNetSocket("http", AF_INET, SOCK_STREAM, 0);
    char *request_payload = malloc(1024);
    char *host_ip = malloc(20);

    if(sock < 0) {
        return -1;
    }

    if(sceNetConnect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        return -1;
    }

    sceNetInetNtop(AF_INET, &ip, host_ip, 20);
    snprintf(request_payload, 1024,
        "GET /set_activity HTTP/1.0\r\n"
        "Host: %s:%d\r\n"
        "Accept: */*\r\n"
        "User-Agent: SimpleRequest (PlayStation 4)\r\n"
        "\r\n"
    );
    
    sceNetSend(sock, request_payload, 1024, 0);

    free(request_payload);
    free(host_ip);
}

// todo: request_loop_game(char *ip, int port)