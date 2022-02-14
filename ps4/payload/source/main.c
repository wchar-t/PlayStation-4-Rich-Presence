#include <ps4.h>
#include "utils.h"

int _main() {
    initKernel();
    initLibc();
    jailbreak();
    initSysUtil();
    initNetwork();

    sceSysUtilSendSystemNotificationWithText(222, "Awaiting host connection on port 4747");
    unsigned int host_ip = get_host_ip();


    return 0;
}