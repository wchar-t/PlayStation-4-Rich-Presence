#include <ps4.h>

unsigned int get_host_ip() {
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
    }
    if(sceNetBind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        sceSysUtilSendSystemNotificationWithText(222, "Can't bind TCP socket");
    }
    if(sceNetListen(server_sock, 5) < 0) {
        sceSysUtilSendSystemNotificationWithText(222, "Can't listen TCP socket");
    }

    int client_addr_size = sizeof(client_addr);
    client_sock = sceNetAccept(server_sock, (struct sockaddr*)&client_addr, (unsigned int*)&client_addr_size);

    if(client_sock < 0) {
        sceSysUtilSendSystemNotificationWithText(222, "Can't accept TCP connection");
    }

    sceSysUtilSendSystemNotificationWithText(222, "Connection accepted");

    sceNetSocketClose(client_sock);
    sceNetSocketClose(server_sock);

    unsigned int host_ip = client_addr.sin_addr.s_addr;
    char *buff = malloc(100);
	char *ip = malloc(20);

	sceNetInetNtop(AF_INET, &host_ip, ip, 20);
	snprintf(buff, 100, "Connection from: %s", ip);
	sceSysUtilSendSystemNotificationWithText(222, buff);

	free(buff);
	free(buff);

	return host_ip;
}