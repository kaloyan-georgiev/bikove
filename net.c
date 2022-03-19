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



int send_all(int sockfd, char* msg, size_t len) {
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

char* receive_all(int sockfd) {
	int res, offset = 0, readsize = 50, bufsize=300;
	char* buf = malloc(bufsize);
	while((res = recv(sockfd, buf+offset, readsize, 0)) == readsize) {
		if(res < 0) {
			perror("Error on receiving");
		}
		offset += res;
		if(offset+readsize >= bufsize) {
			buf = realloc(buf, (bufsize*= 2));
		}
	}
	return buf;
}

int sendstr(int sockfd, char* str) {
	return send_all(sockfd, str, strlen(str)+1);
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

void host_game() {
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

	socklen_t client_socklen;
	printf("Waiting for other player on %d:%d\n", ADDRESS, PORT);
	//second argument is queue length
	fflush(stdout);
	int clcon;
	
	
	listen(socketdes, 1);
	
	if((clcon = accept(socketdes, (struct sockaddr*)&client_address, &client_socklen)) < 0) {
		perror("Error on accepting requests");
		exit(1);
	};
	
	printf("Accepted request from %u:%d\n", ntohs(client_address.sin_addr.s_addr), ntohs(client_address.sin_port));
	
	if(sendstr(clcon, "B: SUC") < 0) {
		perror("Error on sending acknowledgement to client");	
	}
	printf("Sent acknowledgement\n");
	fflush(stdout);

	{
		char* received = receive_all(clcon);
		printf("%s", received);
		free(received);
	}
	
	//listen(socketdes, 0);
	
	{
		char* received = receive_all(clcon);
		printf("%s", received);
		free(received);
	}
	
	close(socketdes);
	close(clcon);
}

void join_game(unsigned int server_address, int server_port) {
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
	if(strcmp(checkrec, "B: SUC") == 0) {
		printf("Successfully connected to %u:%d\n", server_address, server_port);
	} else {
		printf("Bad connection\n");
	}
	free(checkrec);
	fflush(stdout);
	char message[] = "Hello, abcdefghijklmnop\n";
	send_all(clientdes, message, strlen(message)+1);
	printf("Sent message to %u:%u\n", server_socket.sin_addr.s_addr, ntohs(server_socket.sin_port));
	
	char message1[] = "Second message from player2\n";
	send_all(clientdes, message1, strlen(message1)+1);
	printf("Sent message 2 to %u:%u\n", server_socket.sin_addr.s_addr, ntohs(server_socket.sin_port));
	
	close(clientdes);
}

