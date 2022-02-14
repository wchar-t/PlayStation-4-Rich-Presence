#include <ps4.h>

int _main() {
    initKernel();
    initLibc();
    jailbreak();
    initSysUtil();
    initNetwork();
    
    return 0;
}