#ifndef __UTILS_PL__
#define __UTILS_PL__

typedef struct {
    uint32_t d_fileno;
    uint16_t d_reclen;
    uint8_t d_type;
    uint8_t d_namlen;
    char d_name[160 + 1];
} SceKernelDirent;

extern unsigned int get_host_ip();
extern int request_get(char *ip, int port, char *path, char *buff, size_t buff_size);
extern int send_sandbox(char *ip, int port);

#endif