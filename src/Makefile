CC = gcc
FLAGS = -Wall -Wextra -Werror
FLAGS = 
D_OS =
OS =
TEST_LIB = s21_decimal
TESTFILE = s21_decimal_tests
GCOV_FLAG = -fprofile-arcs -ftest-coverage
LIBS_FOR_CHECK_PCKG =

ifeq ($(shell uname), Darwin)
	OS = MacOS
	LIBS_FOR_CHECK_PCKG = -lcheck
	D_OS = -D __My_APPLE__
endif
ifeq ($(shell uname), Linux)
	OS = Linux
	LIBS_FOR_CHECK_PCKG = -lcheck -lsubunit -lm -lpthread -lrt
	D_OS = -D __My_linux__
endif

all: clean s21_decimal.a test gcov_report

a: cls msg clean linter cppcheck s21_decimal.a test gcov_report

test: build_test run_test

clean:
	rm -f *.a *.o *.so *.out $(TESTFILE) *.info *.gcno *.gcda
	rm -rf report	

build_test: s21_decimal.a
	$(CC) $(FLAGS) $(TEST_LIB).a arithmetic_functions.c compare_functions.c conversion_functions.c other_functions.c s21_utils.c $(TESTFILE).c $(LIBS_FOR_CHECK_PCKG) -o $(TESTFILE).out

run_test:
	./$(TESTFILE).out

gcov_report:
	$(CC) $(D_OS) -c $(FLAGS) $(GCOV_FLAG) $(TEST_LIB).c -o $(TEST_LIB).a
	$(CC) $(FLAGS) $(GCOV_FLAG) $(TEST_LIB).a arithmetic_functions.c compare_functions.c conversion_functions.c other_functions.c s21_utils.c $(TESTFILE).c $(LIBS_FOR_CHECK_PCKG) -o gcov.out
	@echo "****** gcov start ******"
	./gcov.out
	lcov -o test.info -c -d .
	genhtml -o report test.info

s21_decimal.a: remove_s21_decimal.a
	@echo "****** s21_decimal lib compile ******"
	$(CC) $(D_OS) -c $(FLAGS) arithmetic_functions.c -o arithmetic_functions.o
	$(CC) $(D_OS) -c $(FLAGS) compare_functions.c -o compare_functions.o
	$(CC) $(D_OS) -c $(FLAGS) conversion_functions.c -o conversion_functions.o
	$(CC) $(D_OS) -c $(FLAGS) other_functions.c -o other_functions.o  
	$(CC) $(D_OS) -c $(FLAGS) s21_utils.c -o s21_utils.o

	ar rcs $(TEST_LIB).a arithmetic_functions.o compare_functions.o conversion_functions.o other_functions.o s21_utils.o

remove_s21_decimal.a:
	@echo "****** remove previous s21_decimal lib ******"
	rm -f *$(TEST_LIB)*.a *$(TEST_LIB)*.o

cls:
	clear

linter:
	@echo "****** linter start ******"
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c *.c
	python3 ../materials/linters/cpplint.py --extensions=h *.h
	rm -f CPPLINT.cfg

cppcheck:
	@echo "****** cppcheck start ******"
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.h

msg:
	@echo "********** Start compiling under $(OS) **********"
	

dev:
	@clear
	gcc arithmetic_functions.c conversion_functions.c s21_utils.c compare_functions.c other_functions.c dev.c
	./a.out

push:
	git add *
	git commit -m "extra"
	git push origin larwyn

