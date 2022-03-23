#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include "nums.h"

#define NETWORK 1
#define NUMBERS 2

int SOCKET_DES;

void close_server() {
	printf("Closing server...\n");
	close(SOCKET_DES);
}

ssize_t send_all(int socketdes, void* message, size_t msglen) {
	ssize_t snt;
	ssize_t sent=0;
	while((snt = send(socketdes, message, msglen, 0)) < msglen) {
		if(snt < 0) {
			return -1;
		}
		sent += snt;
		printf("%lu bytes sent\n", snt);
	}
	printf("TOTAL: %lu bytes sent\n", msglen);
	return sent;
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

void host_game() {
	int port, socketdes = socket(PF_INET, SOCK_STREAM, 0);
	if(socketdes < 0) {
		perror("Error on creating socket");
		exit(1);
	}
	struct sockaddr_in client_address;
	bzero((char*)&client_address, sizeof(client_address));
	port = 1047;


	SOCKET_DES = socketdes;
	atexit(close_server);
	
	printf("Binding to address %d:%d\n", INADDR_ANY, port);
	if(bind_socket(socketdes, INADDR_ANY, port) < 0) {
		perror("Error on binding socket");
		//close(socketdes);
		exit(1);
	}

	socklen_t client_socklen;
	printf("Waiting for other player on address %d:%d\n", INADDR_ANY, port);
	//second argument is queue length
	listen(socketdes, 5);
	
	if(accept(socketdes, (struct sockaddr*)&client_address, &client_socklen) < 0) {
		perror("Error on accepting requests");
		exit(1);
	};
	
	listen(socketdes, 1);
	char message[128];
	int rec, rlen =0;
	while((rec = recv(socketdes, &message, 128, 0)) > 0 ) {
		if(rec < 0) {
			perror("Error receiving data from client\n");
		}
		printf("%d bytes received\n", rec);
		rlen += rec;
	};

	message[rlen] = '\0';
	printf("Message from other side : %s\n", message);
	//int clsocketdes = socket(PF_INET, SOCK_STREAM, 0);
	/*if(bind(clsocketdes, (struct sockaddr*)&client_address, client_socklen) < 0) {
		perror("Error on binding client socket");
	};*/
	send(socketdes, "ACK", 3, 0);
	
	
}

void join_game(unsigned int server_address, int server_port) {
	int clport, clientdes = socket(PF_INET, SOCK_STREAM, 0);
	if(clientdes < 0) {
		perror("Error on creating socket");
		exit(1);
	}
	struct sockaddr_in server_socket ,client_socket;
	bzero((char*)&server_socket, sizeof(server_socket));
	bzero((char*)&client_socket, sizeof(client_socket));

	clport = 1044;
	
	struct in_addr address = {INADDR_ANY};
	client_socket.sin_family = AF_INET;
	client_socket.sin_addr = address;
	client_socket.sin_port = htons(clport);
	
	struct in_addr address_s = {server_address};
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr = address_s;
	server_socket.sin_port = htons(server_port);
	
	
	if(bind(clientdes, (struct sockaddr*) &client_socket, sizeof(client_socket)) < 0) {
		perror("Error on binding socket");
		exit(1);
	}
	
	if(connect(clientdes, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0) {
		perror("Error on connecting to server");
		exit(1);
	}
	printf("Successfully connected to %d:%d\n", server_address, server_port);
	//char message[] = "Zdrasti";
	if(send_all(clientdes, "zdrasti", strlen("zdrasti")) < 0) {
		perror("Failed to send message to server");
	}
	
	listen(clientdes, 1);
	char received[128];
	int rec, rlen = 0;
	while((rec = recv(clientdes, &received, 128, 0)) > 0 ) {
		if(rec < 0) {
			perror("Error receiving data from client\n");
		}
		rlen += rec;
	};
	received[rlen] = '\0';
	printf("Got message from other side: %s\n", received);
	close(clientdes);
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
