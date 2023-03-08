build: tema1.c
	gcc -Wall -o editor tema1.c -lm -std=c11 -ggdb3
	

.PHONY : clean
clean:
	rm -f editor
	rm -f .out