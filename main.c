#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include "net.h"

int main() {
	printf("Choose playing mode (h - host, j - join):");
	//printf("%d", INADDR_ANY);
	char c = getchar();
	switch(c) {
		case 'h':
			host_game();
			break;
		case 'j':
			join_game(INADDR_ANY, 1047);
			break;
	}
}
