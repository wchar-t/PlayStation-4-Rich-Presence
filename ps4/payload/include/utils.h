#ifndef __UTILS_PL__
#define __UTILS_PL__

extern unsigned int get_host_ip();
extern int request_get(char *ip, int port, char *path, char *buff, size_t buff_size);
extern void request_loop_game(char *ip, int port);

#endif