# the compiler to use
CC = clang

# include directory for header files
IDIR=./include

# optimization level
OPTIMIZATION=-O3

# compiler flags:
#  -g adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -Wextra turns on some extra warning flags that are not enabled by -Wall
#  -Werror turns all warnings into errors
#  -Wpedantic turns on pedantic warnings
#  -std=c99 specifies that the code should be compiled according to the C99 standard
CFLAGS=-I$(IDIR) -g -Wall -Wextra -Werror -Wpedantic -std=c99 $(OPTIMIZATION)

# the object files to include in the executable
OBJ=./obj/main.o ./obj/scanner.o ./obj/chunk.o ./obj/memory.o ./obj/debug.o ./obj/value.o ./obj/vm.o ./obj/compiler.o

# the executable file to create
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