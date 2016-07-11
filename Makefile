CC = gcc
FLAGS = -g -Wall -Wpedantic -Wextra
SOURCE = src/*.c
OUTPUT = virtualmem

all: $(SOURCE)
	$(CC) $(FLAGS) -o $(OUTPUT) $(SOURCE)

clean:
	@rm -rf $(OUTPUT)*
