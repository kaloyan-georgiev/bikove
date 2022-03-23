#ifndef NET_HH
#define NET_HH
#include <stddef.h>
int host_game();
int join_game(unsigned int, int);
int send_all(int sockfd, void* msg, size_t len);
void* receive_all(int sockfd);

#endif
