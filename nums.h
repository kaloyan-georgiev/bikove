#ifndef NUMS_HH
#define NUMS_HH

struct answer_t {
	int kravi;
	int bikove;
};

struct answer_t check_number(int number, int guess, int dc);
int enter_number(int dc, int repeat);
#endif
