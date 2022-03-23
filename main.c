#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "net.h"
#include "nums.h"
#include "list.h"

#define NETWORK 1
#define NUMBERS 2
char** parse_cmdline(char* cmdline);
void complex_free(char*** tofree);
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
	
		printf("Choose playing mode (h <number length> - host, j <ip> <port> - join):\n");

		char accept = 1;
		while(accept) {
			char command[127];
			fgets(command, 127, stdin);
			char** arguments = parse_cmdline(command);
			//If input is only \n
			if(arguments[0][0] == 0) {
				continue;
			}
			accept = 0;
			int argslen = 0;
			for(int i=0; arguments[i] != NULL; i++) {
				argslen++;
			}
			if(strcmp(arguments[0], "h") == 0) {
				if(argslen < 2) {
					printf("Enter number length\n");
					accept = 1;
					continue;
				}
				int NUMLEN = atoi(arguments[1]);
				int socket = host_game();
				send_all(socket, &NUMLEN, sizeof(NUMLEN));
				printf("Now enter your number with length %d:\n", NUMLEN);
				int repeat = 0;
				int MYNUM = enter_number(NUMLEN, repeat);
				printf("YOUR NUMBER IS %d\n", MYNUM);
				printf("Host is first to guess\n");	
										
				int RUNNING = 1, STOPPED = 0;
				int game = RUNNING;
				struct list_t guesses;
				int guess_count = 0;
				
				while(game == RUNNING) {
					printf("Enter guess no %d for your opponent's number:\n", ++guess_count);
					int guess = enter_number(NUMLEN, repeat);
					//guesses.push_back(guess);
					send_all(socket, &guess, sizeof(guess));
					struct answer_t* answer_back = receive_all(socket);
					printf("Your guess %d got %d bulls and %d cows\n", guess, answer_back->bikove, answer_back->kravi);
					printf("Waiting for opponent's guess...\n");
					
					int* opponent_guess = receive_all(socket);
					printf("Your opponent's guess is %d\n", *opponent_guess);
					struct answer_t answer = check_number(MYNUM, *opponent_guess, NUMLEN);
					send_all(socket, &answer, sizeof(answer));
					free(opponent_guess);
					
				}
				close(socket);
			}
			else if(strcmp(arguments[0], "j") == 0) {
				int socket = join_game(0, 1047);
				int* NUMLEN = receive_all(socket);
				printf("Required number length is %d\n", *NUMLEN);
				printf("Now enter your number with length %d:\n", *NUMLEN);
				int repeat = 0;
				int MYNUM = enter_number(*NUMLEN, repeat);	
				printf("YOUR NUMBER IS %d\n", MYNUM);
				printf("Host is first to guess\n");	
										
				int RUNNING = 1, STOPPED = 0;
				int game = RUNNING;
				struct list_t guesses;
				int guess_count = 0;
				
				while(game == RUNNING) {
					printf("Waiting for opponent's guess...\n");
					int* opponent_guess = receive_all(socket);
					printf("Your opponent's guess is %d\n", *opponent_guess);
					struct answer_t answer = check_number(MYNUM, *opponent_guess, *NUMLEN);
					send_all(socket, &answer, sizeof(answer));
					free(opponent_guess);
				
					printf("Enter guess no %d for your opponent's number:\n", ++guess_count);
					int guess = enter_number(*NUMLEN, repeat);
					//guesses.push_back(guess);
					send_all(socket, &guess, sizeof(guess));
					struct answer_t* answer_back = receive_all(socket);
					printf("Your guess %d got %d bulls and %d cows\n", guess, answer_back->bikove, answer_back->kravi);
					
				}
				free(NUMLEN);
				close(socket);
			}
			else {
				accept = 1;
				printf("Invalid command\n");
			}
			complex_free(&arguments);
		}
	}
	exit(0);
}
