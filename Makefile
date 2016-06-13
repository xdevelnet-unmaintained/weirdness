COMPILER=cc
OPTIONS=-std=c99 -Wall -Wextra

all:
	$(COMPILER) $(OPTIONS) ramtest.c -o ramtest

clean:
	rm ramtest

clear:
	rm ramtest
