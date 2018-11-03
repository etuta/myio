myio: myio.c
	gcc -g -Wall -pedantic -o myio myio.c

.PHONY: clean
clean:
	rm -f myio
