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

OPTIMIZATION=-O3
CFLAGS=-I$(IDIR) -g -Wall -Wextra -Werror -Wpedantic -std=c99 $(OPTIMIZATION)

OBJ=./obj/main.o ./obj/scanner.o

TARGET=./out/prog.elf

prog: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: run
run:
	$(TARGET)