# the compiler to use
CC = clang

# include directory for header files
IDIR=./include

# optimization level
OPTIMIZATION=-O0

# compiler flags:
#  -g adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -Wextra turns on some extra warning flags that are not enabled by -Wall
#  -Werror turns all warnings into errors
#  -Wpedantic turns on pedantic warnings
#  -std=c99 specifies that the code should be compiled according to the C99 standard
CFLAGS=-I$(IDIR) -g -Wall -Wextra -Werror -Wpedantic -std=c99 $(OPTIMIZATION)

LINKER_FLAGS=-lm -lpthread -lSDL2 -lSDL2main

# the object files to include in the executable
OBJ=./obj/main.o		\
	./obj/scanner.o 	\
	./obj/chunk.o		\
	./obj/memory.o		\
	./obj/debug.o		\
	./obj/value.o		\
	./obj/vm.o			\
	./obj/compiler.o	\
	./obj/object.o		\
	./obj/table.o		\
	./obj/fileIO.o

# the executable file to create
TARGET=./out/prog.elf

prog: $(OBJ) ./obj/vm.o
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LINKER_FLAGS)

./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
# use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
# ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

.PHONY: run
run: prog
	$(TARGET) $(RUN_ARGS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ)
	rm -f $(TARGET)

.PHONY: cleanmake
cleanmake:
	make clean
	make

.PHONY: cleanrun
cleanrun:
	make clean
	make run