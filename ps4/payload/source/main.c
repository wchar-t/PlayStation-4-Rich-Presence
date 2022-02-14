#include <ps4.h>

#define debug(sock, format, ...)\
	do {\
		char buffer[512];\
		int size = sprintf(buffer, format, ##__VA_ARGS__);\
		sceNetSend(sock, buffer, size, 0);\
	} while(0)

int _main() {
    initKernel();
    initLibc();
    jailbreak();
    initSysUtil();
    initNetwork();
	
    char socketName[] = "debug";

	struct sockaddr_in server;

	server.sin_len = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = IP(192, 168, 100, 50);
	server.sin_port = sceNetHtons(4747);
	memset(server.sin_zero, 0, sizeof(server.sin_zero));

	int sock = sceNetSocket(socketName, AF_INET, SOCK_STREAM, 0);
	sceNetConnect(sock, (struct sockaddr *)&server, sizeof(server));

	debug(sock, "oo the PID: %d", syscall(20));


	openBrowser("https://crunchy-dl.com");

	sceNetSocketClose(sock);
    
    return 0;
}