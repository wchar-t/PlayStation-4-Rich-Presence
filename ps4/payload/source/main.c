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

    sceSysUtilSendSystemNotificationWithText(222, host_ip);

    request_get(host_ip, 4848, "/set_activity", &buff, 1024);

    free(host_ip);

    return 0;
}