CFLAGS = -std=gnu99 -g -Wall -Wextra -Werror -pedantic
LD_FLAGS = -lpthread -lrt
PROGS = proj2

all: $(PROGS)

clean:
	rm *.o
	rm proj2.out

run: $(PROGS)
	./proj2 1 1 1 1 10

print:
	cat proj2.out
	rm proj2.out

proj2: main.o printf_flush.o fprintf_flush.o perror_flush.o free_resources.o create_officers.o create_customers.o serve_line.o take_break.o enter_for_service.o check_arguments.o close_office.o init_semaphores.o
	gcc $(CFLAGS) -o $@ $^ $(LD_FLAGS)

main.o: main.c

init_semaphores.o: init_semaphores.c

close_office.o: close_office.c

check_arguments.o: check_arguments.c

enter_for_service.o: enter_for_service.c

take_break.o: take_break.c

serve_line.o: serve_line.c

create_customers.o: create_customers.c

create_officers.o: create_officers.c

printf_flush.o: printf_flush.c

fprintf_flush.o: fprintf_flush.c

perror_flush.o: perror_flush.c

free_resources.o: free_resources.c