CC = gcc
CFLAGS =-Wall -Werror -Wextra -Wpedantic -g -std=c++17
UI_SRC = $(wildcard ./interface/*.cpp, ./interface/*.h)
CONTROLLER_SRC = $(wildcard ./controller/*.cpp, ./controller/*.h)
MODEL_SRC = $(wildcard ./model/*.cpp)
MODEL_H = $(wildcard ./model/*.h)
MODEL_OBJ = $(patsubst %.cpp, %.o, $(MODEL_SRC))
MODEL_OBJ_COV = $(patsubst %.cpp, %.o_cov, $(MODEL_SRC))
TEST_SRC = $(wildcard ./tests/*.cpp)
TEST_H = $(wildcard ./tests/*.h)
TEST_OBJ = $(patsubst %.cpp, %.o, $(TEST_SRC))

TEST_BUILD_DIR = build_test
CMEMTEST = valgrind --leak-check=full --track-origins=yes
# START Appears in root CMakeLists.txt
TEST_FILE = model_test
CLIB = s21_calculator_model.a
CLIB_DIR = libs
# END Appears in root CMakeLists.txt

#CCFLAGS = -L ./ -l:$(CLIB)
QMAKE = /Users/alexvnderftw/Qt/6.6.0/macos/bin/qmake
OUTPUT_DIR = SmartCalc_v2.0
APP_LABEL = SmartCalc_v2
APP_MOVE = interface/SmartCalc_v2

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
APP_MOVE = interface/SmartCalc_v2.app/Contents/MacOS/SmartCalc_v2
#CCFLAGS = $(CLIB)
CMEMTEST = leaks -atExit --
endif


.PHONY: all s21_calculator_model.a s21_calculator_model_cov.a gcov_report test style style_fix memtest clean rebuild

# Main targets

all: install

install: uninstall libs
	cd interface && $(QMAKE) && make
	mkdir -p $(OUTPUT_DIR)
#cp README.md ../$(APP_FILE)
#cp README_RUS.md ../$(APP_FILE)
	mv $(APP_MOVE) $(OUTPUT_DIR)/

run:
	./$(OUTPUT_DIR)/$(APP_LABEL)

uninstall:
#rm -rf ../$(APP_FILE)

dist: clean distclean
	tar -cf 3DViewer_v1.0.tar 3DViewer_v1 Makefile README.md 3DViewer_v1.0.h modules misc tests images

dvi:
	open README.md

dvi_rus:
	open README_RUS.md

gcov_report: test
	lcov -t "S21CalculatorCPP" -o s21_calculator_model.info -c -d ./model/
	lcov -r s21_calculator_model.info '/usr/include/*' -o s21_calculator_model.info
	genhtml -o report s21_calculator_model.info
	open ./report/index.html

test: s21_calculator_model_cov.a
	cmake -S . -B $(TEST_BUILD_DIR)
	cmake --build $(TEST_BUILD_DIR)
	mv ./$(TEST_BUILD_DIR)/$(TEST_FILE) ./
	./$(TEST_FILE)

style: clean
	clang-format -style=Google -n $(MODEL_SRC) $(MODEL_H) $(TEST_SRC) $(TEST_H) $(UI_SRC) $(CONTROLLER_SRC)

memtest: clean test
	$(CMEMTEST) ./$(TEST_FILE)

clean:
	rm -rf $(TEST_FILE)
	rm -f ./*.o ./*.o_cov ./tests/*.o ./*.a ./model/*.o_cov ./model/*.o
	rm -rf ./*.gcda ./*.gcno ./*.info ./model/*.gcda ./model/*.gcno ./model/*.info
	rm -rf ./report/
	rm -rf ./$(TEST_BUILD_DIR)
	rm -rf ./$(CLIB_DIR)
	rm -rf interface/.qmake.stash interface/Makefile interface/moc_*
	rm -rf interface/ui_*.h interface/*.o interface/$(APP_LABEL).app interface/qrc_resource.cpp

rebuild: clean all

# Misc targets

libs: s21_calculator_model.a

s21_calculator_model.a: $(MODEL_OBJ)
	mkdir -p $(CLIB_DIR)
	rm -rf $(CLIB_DIR)/$(CLIB)
	ar -rs $(CLIB) $(MODEL_OBJ)
	mv $(CLIB) ./$(CLIB_DIR)/

s21_calculator_model_cov.a: $(MODEL_OBJ_COV)
	mkdir -p $(CLIB_DIR)
	rm -rf $(CLIB_DIR)/$(CLIB)
	ar -rs $(CLIB) $(MODEL_OBJ_COV)
	mv $(CLIB) ./$(CLIB_DIR)/

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o_cov: %.cpp
	$(CC) --coverage $(CFLAGS) -c $< -o $@

style_fix: clean
	clang-format -style=Google -i $(MODEL_SRC) $(MODEL_H) $(TEST_SRC) $(TEST_H) $(UI_SRC) $(CONTROLLER_SRC)
