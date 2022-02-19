#include <ps4.h>
#include "utils.h"

int _main() {
    initKernel();
    initLibc();
    jailbreak();
    initSysUtil();
    initNetwork();
    
    char buff[1024];

    sceSysUtilSendSystemNotificationWithText(222, "Awaiting host connection on port 4747");
    char *host_ip = malloc(20);
    get_host_ip(host_ip);
    sceKernelSleep(5);

    while(1) {
        if(send_sandbox(host_ip, 4848) < 0) {
            break;
        }
        sceKernelSleep(1);
    }
    
    sceSysUtilSendSystemNotificationWithText(222, "Server closed. Exiting discord activities...");
    free(host_ip);
    
    return 0;
}