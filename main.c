#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

void host_game() {
	int port, socketdes = socket(PF_INET, SOCK_STREAM, 0);
	if(socketdes < 0) {
		perror("Error on creating socket");
		exit(1);
	}
	struct sockaddr_in server_address, client_address;
	bzero((char*)&server_address, sizeof(server_address));
	bzero((char*)&client_address, sizeof(client_address));
	port = 1047;
	
	struct in_addr address = {INADDR_ANY};
	server_address.sin_family = AF_INET;
	server_address.sin_addr = address;
	server_address.sin_port = htons(port);
	
	if(bind(socketdes, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
		perror("Error on binding socket");
		close(socketdes);
		exit(1);
	}

	socklen_t client_socklen;
	printf("Waiting for other player on address %d port %d", INADDR_ANY, port);
	//second argument is queue length
	listen(socketdes, 5);
	
	if(accept(socketdes, (struct sockaddr*)&client_address, &client_socklen) < 0) {
		perror("Error on accepting requests");
		exit(1);
	};
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
}

int main() {
	printf("Choose playing mode (h - host, j - join):");
	//printf("%d", INADDR_ANY);
	char c = getchar();
	switch(c) {
		case 'h':
		host_game();
		break;
		case 'j':
		join_game;
		break;
	}
}
