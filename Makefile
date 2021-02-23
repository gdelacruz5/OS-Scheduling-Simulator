schedSim : main.c parseFile.c simulator.c linkedList.c
	gcc -Wall -Werror -o $@ *.c -I.
