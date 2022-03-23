#include <stdio.h>
#include <math.h>
#include "list.h"
#include "nums.h"

int digit_count(int n) {
	int count = 0;
	while(n>0) {
		n /= 10;
		count++;
	}
	return count;
}

char get_digit(int num, int n) {
	return (num / (int)pow(10, n)) % 10;
}

struct list_t num_to_list(int num, int dc) {
	struct list_t list = {NULL};
	for(int i=0; i < dc; i++) {
		push_back(&list, get_digit(num, i));
	}
	return list;
}

//trqbva nu opravqne
int enter_number(int dc, int repeat) {
	int num = 0;
	if(repeat >= dc) {
		return -1;
	}
	scanf("%d", &num);
	while(digit_count(num) != dc) {
		printf("ERROR: Enter a number with %d digits\n", dc);
		scanf("%d", &num);
	}
	return num;
}

int check_bikove(int number, int guess, int dc) {
	int bikove = 0;
	for(int i=0; i < dc; i++) {
		if(get_digit(guess, i) == get_digit(number, i)) {
			bikove++;
		};
		
	}
	return bikove;
}


int check_kravi(int number, int guess, int dc) {
	int kravi = 0, matches=0;
	struct list_t numl = num_to_list(number, dc);
	for(int i=0; i < dc; i++) {
		char digit = get_digit(guess, i);
		struct node_t* current = numl.head;
		while(current != NULL) {
			if(current->value == digit) {
				struct node_t* tofree = current;
				current = current->next;
				detach(&numl, tofree);
				matches++;
				break;
			}
			current = current->next;
		}
	}
	destroy_list(&numl);
	kravi = matches - check_bikove(number, guess, dc);
	return kravi;	
}


struct answer_t check_number(int number, int guess, int dc) {
	struct answer_t answer = {0, 0};
	answer.kravi = check_kravi(number, guess, dc);
	answer.bikove = check_bikove(number, guess, dc);
	return answer;
}
