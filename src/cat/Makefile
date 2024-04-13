CC = gcc
CFLAGS = -Wall -Werror -Wextra -g

all: s21_cat

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) -o s21_cat s21_cat.c

build: clean all

rebuild: build
style_check:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c
	clang-format -i *.c
	rm -rf .clang-format

clean:
	rm -f s21_cat
