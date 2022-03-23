#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net.h"
#include "nums.h"

#define NETWORK 1
#define NUMBERS 2

int SOCKET_DES;

void close_server() {
	printf("Closing server...\n");
	close(SOCKET_DES);
}

int bind_socket(int descriptor, unsigned int address, unsigned int port) {
	struct sockaddr_in address_s;
	bzero((char*)&address_s, sizeof(address_s));
	struct in_addr iaddress = {address};
	address_s.sin_family = AF_INET;
	address_s.sin_addr = iaddress;
	address_s.sin_port = htons(port);
	if(bind(descriptor, (struct sockaddr*)&address_s, sizeof(address_s)) < 0) {
		return -1;
	}
	return 0;
}


int main() {
	int test_mode = NETWORK;
	if(test_mode == NUMBERS) {
		printf("Enter number 1:");
		int number = enter_number(5, 1);
		printf("Enter number 2:");
		int guess = enter_number(5, 1);
		struct answer_t ans = check_number(number, guess, 5);
		printf("Kravi - %d, Bikove - %d\n", ans.kravi, ans.bikove);	
	}
	else if(test_mode == NETWORK) {	
	
		printf("Choose playing mode (h - host, j - join):\n");
		//printf("%d", INADDR_ANY);
		char accept = 1;
		while(accept) {
			char c = getchar();
			if(c == '\n') {
				continue;
			}
			accept = 0;
			switch(c) {
				case 'h':
					host_game();
					break;
				case 'j':
					join_game(INADDR_ANY, 1047);
					break;
				default:
					accept = 1;
					printf("Invalid command\n");
				break;
			}
		}
	}
	exit(0);
}
