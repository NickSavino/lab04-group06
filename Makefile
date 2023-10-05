.PHONY: all
all: scheduler.out

scheduler.out: scheduler.c
	gcc -Wall -o scheduler.out scheduler.c

.PHONY: clean
clean: 
	rm -f *.out
