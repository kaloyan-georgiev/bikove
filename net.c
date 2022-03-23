#include "net.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>


char* ACK_MSG = "BSUC";

int send_all(int sockfd, void* msg, size_t len) {
	int res;
	if((res = send(sockfd, msg, len, 0)) < len) {
		if(res < 0) {
			return -1;
		}
		else {
			while((res = send(sockfd, msg, len, 0)) != 0);
		}
	}
	return 0;
}

void* receive_all(int sockfd) {
	int res, bufsize=255;
	void* buf = malloc(bufsize);
	if((res = recv(sockfd, buf, bufsize, 0)) < 0) {
		perror("receive_all: Error on receiving");
	}
	return buf;
}

int sendstr(int sockfd, char* str) {
	return send_all(sockfd, str, strlen(str)+1);
}

void print_message(int socketdes) {
	char* received = receive_all(socketdes);
	printf("%s", received);
	free(received);
}

struct sockaddr_in new_Isockaddr(unsigned int address, int port) {
	
	struct sockaddr_in socket_address;
	bzero((char*)&socket_address, sizeof(socket_address));
	struct in_addr Iaddress = {address};
	
	socket_address.sin_family = AF_INET;
	socket_address.sin_addr = Iaddress;
	socket_address.sin_port = htons(port);
	
	return socket_address;
}

int host_game() {
	int PORT, socketdes = socket(PF_INET, SOCK_STREAM, 0);
	if(socketdes < 0) {
		perror("Error on creating socket");
		exit(1);
	}
	
	PORT = 1047;
	unsigned int ADDRESS = INADDR_ANY;
	
	struct sockaddr_in client_address;
	struct sockaddr_in server_address = new_Isockaddr(ADDRESS, PORT);
	
	bzero((char*)&client_address, sizeof(client_address));
	
	if(bind(socketdes, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
		perror("Error on binding socket");
		close(socketdes);
		exit(1);
	}
	
	int clcon;
	socklen_t client_socklen;
	printf("Waiting for other player on %d:%d\n", ADDRESS, PORT);
	fflush(stdout);
	
	//second argument is queue length
	listen(socketdes, 1);
	
	if((clcon = accept(socketdes, (struct sockaddr*)&client_address, &client_socklen)) < 0) {
		perror("Error on accepting requests");
		exit(1);
	};
	close(socketdes);
	printf("Accepted request from %u:%d\n", ntohs(client_address.sin_addr.s_addr), ntohs(client_address.sin_port));
	
	if(sendstr(clcon, ACK_MSG) < 0) {
		perror("Error on sending acknowledgement to client");	
	}
	printf("Sent acknowledgement\n");
	fflush(stdout);

	{
		char* received = receive_all(clcon);
		printf("%s", received);
		free(received);
	}
	
	

	sendstr(clcon, "Hello from server!\n");
	
	return clcon;
}

int join_game(unsigned int server_address, int server_port) {
	int THIS_PORT, clientdes = socket(PF_INET, SOCK_STREAM, 0);
	if(clientdes < 0) {
		perror("Error on creating socket");
		exit(1);
	}
		
	THIS_PORT = 1044;
	struct sockaddr_in server_socket = new_Isockaddr(server_address, server_port);
	struct sockaddr_in client_socket = new_Isockaddr(0, THIS_PORT);
	
	if(bind(clientdes, (struct sockaddr*) &client_socket, sizeof(client_socket)) < 0) {
		perror("Error on binding socket");
		close(clientdes);
		exit(1);
	}
	
	if(connect(clientdes, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0) {
		perror("Error on connecting to server");
		close(clientdes);
		exit(1);
	}
	printf("Connected to %u:%d\n", server_address, server_port);
	
	char* checkrec = receive_all(clientdes);
	printf("Received from host: %s\n", checkrec);
	if(strcmp(checkrec, ACK_MSG) == 0) {
		printf("Successfully connected to %u:%d\n", server_address, server_port);
	} else {
		printf("Bad connection\n");
	}
	free(checkrec);
	fflush(stdout);
	sendstr(clientdes, "Hello from client!\n");
	printf("Sent message to %u:%u\n", server_socket.sin_addr.s_addr, ntohs(server_socket.sin_port));
	
	print_message(clientdes);
	printf("Received message from %u:%u\n", server_socket.sin_addr.s_addr, ntohs(server_socket.sin_port));
	
	return clientdes;
}

