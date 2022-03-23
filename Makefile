all:
	gcc -Wall -pedantic --std=c11 main.c net.c list.c nums.c clparse.c -lm
