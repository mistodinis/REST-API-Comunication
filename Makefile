CC=gcc
CFLAGS=-I.

client: tema.c requests.c helpers.c buffer.c
	$(CC) -o tema tema.c requests.c helpers.c buffer.c -Wall -g

run: tema
	./tema

clean:
	rm -f *.o tema
