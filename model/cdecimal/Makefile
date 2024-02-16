CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11
LIB_SRC = $(wildcard ./functions/*.c ./functions/big_decimal/*.c)
LIB_OBJ = $(patsubst %.c, %.o, $(LIB_SRC))
LIB_OBJ_COV = $(patsubst %.c, %.o_cov, $(LIB_SRC))

CHECK = ./tests/include.check $(wildcard ./tests/s21*)
CLIB_FLAGS = -L. -l:s21_decimal.a -lcheck -lrt -lpthread -lsubunit -lm
CLIB = s21_decimal.a
CLEAK = valgrind --leak-check=full -q ./s21_decimal_check

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
CLIB_FLAGS = -L. s21_decimal.a -lcheck
CLIB = s21_decimal.a
CLEAK = leaks -atExit -- ./s21_decimal_check | grep LEAK:
endif

all : s21_decimal.a

gcov_report: test
	./s21_decimal_check
	lcov -t "decimal" -o s21_decimal.info -c -d .
	genhtml -o report s21_decimal.info
	open ./report/index.html

test: s21_decimal.a_cov
	rm -f ./s21_decimal_check
	checkmk clean_mode=1 $(CHECK) > s21_decimal_check.c
	$(CC) --coverage $(CFLAGS) -o s21_decimal_check s21_decimal_check.c $(CLIB_FLAGS)
	./s21_decimal_check

s21_decimal.a : $(LIB_OBJ)
	ar r $(CLIB) $(LIB_OBJ)

s21_decimal.a_cov : $(LIB_OBJ_COV)
	rm -f ./s21_decimal.a
	ar r $(CLIB) $(LIB_OBJ_COV)

style :
	clang-format -style=Google -n ./functions/*.c ./functions/big_decimal/*.c ./functions/big_decimal/*.h

style_fix :
	clang-format -style=Google -i ./functions/*.c ./functions/big_decimal/*.c ./functions/big_decimal/*.h

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@	

%.o_cov : %.c
	$(CC) --coverage $(CFLAGS) -c $< -o $@

mytest : clean s21_decimal.a
	$(CC) -g -c test.c -o test.o 
	$(CC) -o mytest test.o -L. -l:s21_decimal.a

leak_test : rebuild
	$(CLEAK)

clean :
	rm -rf s21_decimal_check.dSYM
	rm -f ./functions/*.o *.a *.o s21_decimal_check* *.check ./functions/big_decimal/*.o
	rm -f ./*.a_cov *.info
	rm -f ./functions/*.gcda ./functions/*.gcno ./functions/*.o_cov
	rm -f ./functions/big_decimal/*.gcda ./functions/big_decimal/*.gcno ./functions/big_decimal/*.o_cov
	rm -rf ./report/

rebuild : clean test