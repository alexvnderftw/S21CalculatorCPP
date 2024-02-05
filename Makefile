CC = gcc
CFLAGS =-Wall -Werror -Wextra -Wpedantic -g -std=c++17
MODEL_SRC = $(wildcard ./model/*.cpp)
MODEL_H = $(wildcard ./model/*.h)
MODEL_OBJ = $(patsubst %.cpp, %.o, $(MODEL_SRC))
MODEL_OBJ_COV = $(patsubst %.cpp, %.o_cov, $(MODEL_SRC))
TEST_SRC = $(wildcard ./tests/*.cpp)
TEST_H = $(wildcard ./tests/*.h)
TEST_OBJ = $(patsubst %.cpp, %.o, $(TEST_SRC))
TEST_FILE = model_test
CMEMTEST = valgrind --leak-check=full --track-origins=yes
CLIB = s21_calculator_model.a
CCFLAGS = -L ./ -l:$(CLIB)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
CCFLAGS = $(CLIB)
CMEMTEST = leaks -atExit --
endif

.PHONY: all s21_calculator_model.a s21_calculator_model_cov.a gcov_report test style style_fix memtest clean rebuild

all: s21_calculator_model.a

s21_calculator_model.a: $(MODEL_OBJ)
	rm -rf $(CLIB)
	ar -rs $(CLIB) $(MODEL_OBJ)

s21_calculator_model_cov.a: $(MODEL_OBJ_COV)
	rm -rf $(CLIB)
	ar -rs $(CLIB) $(MODEL_OBJ_COV)

gcov_report: test
	lcov -t "S21Calculator" -o s21_calculator_model.info -c -d ./model/
	lcov -r s21_calculator_model.info '/usr/include/*' -o s21_calculator_model.info
	genhtml -o report s21_calculator_model.info
	open ./report/index.html

test: s21_calculator_model_cov.a
	cmake -S . -B test_build
	cmake --build test_build
	mv ./test_build/$(TEST_FILE) ./
	./$(TEST_FILE)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o_cov: %.cpp
	$(CC) --coverage $(CFLAGS) -c $< -o $@

style: clean
	clang-format -style=Google -n $(MODEL_SRC) $(MODEL_H) $(TEST_SRC) $(TEST_H)

style_fix: clean
	clang-format -style=Google -i $(MODEL_SRC) $(MODEL_H) $(TEST_SRC) $(TEST_H)

memtest: clean test
	$(CMEMTEST) ./$(TEST_FILE)

clean:
	rm -rf $(TEST_FILE)
	rm -f ./*.o ./*.o_cov ./tests/*.o ./*.a ./model/*.o_cov ./model/*.o
	rm -rf ./*.gcda ./*.gcno ./*.info ./model/*.gcda ./model/*.gcno ./model/*.info
	rm -rf ./report/
	rm -rf ./test_build/

rebuild: clean all