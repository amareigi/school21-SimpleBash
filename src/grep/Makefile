CC = gcc
CFLAGS = -Wall -Werror -Wextra -g

all: s21_grep

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c

build: clean all

rebuild: build

style_check:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c
	clang-format -i *.c
	rm -rf .clang-format

clean:
	rm -f s21_grep
