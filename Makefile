CFLAGS = -std=gnu99 -g -Wall -Wextra -Werror -pedantic
LD_FLAGS = -lpthread -lrt
PROGS = proj2

all: $(PROGS)

clean:
	rm *.o
	rm proj2.out

run: $(PROGS)
	./proj2 1 1 1 1 10

proj2: main.o printf_flush.o fprintf_flush.o perror_flush.o free_resources.o create_officers.o create_customers.o serve_line.o take_break.o
	gcc $(CFLAGS) -o $@ $^ $(LD_FLAGS)

main.o: main.c

take_break.o: take_break.c

serve_line.o: serve_line.c

create_customers.o: create_customers.c

create_officers.o: create_officers.c

printf_flush.o: printf_flush.c

fprintf_flush.o: fprintf_flush.c

perror_flush.o: perror_flush.c

free_resources.o: free_resources.c