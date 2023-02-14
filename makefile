# the compiler to use
CC = clang

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -Wextra turns on some extra warning flags that are not enabled by -Wall
#  -Werror turns all warnings into errors
#  -Wpedantic turns on pedantic warnings
#  -std=c99 specifies that the code should be compiled according to the C99 standard
IDIR=./include
CFLAGS=-I$(IDIR) -g -Wall -Wextra -Werror -Wpedantic -std=c99
OBJDIR=./obj

prog: $(OBJDIR)/main.o $(OBJDIR)/scanner.o
	$(CC) $(CFLAGS) -o ./out/prog $(OBJDIR)/main.o $(OBJDIR)/scanner.o

$(OBJDIR)/scanner.o:
	$(CC) $(CFLAGS) -c ./src/scanner.c -o $(OBJDIR)/scanner.o

$(OBJDIR)/main.o: ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o $(OBJDIR)/main.o

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o ./out/prog

.PHONY: run
run:
	./out/prog