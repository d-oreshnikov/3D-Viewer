CC             =gcc
OS_NAME        =$(shell uname)
CFLAGS         =-Wall -Wextra -Werror -std=c11 -g 
GCOVFLAGS      =-fprofile-arcs -ftest-coverage
LIBSRCS        =./backend/my_affine.c ./backend/parser.c
LIBHDRS        =./backend/my_viewer.h 
TESTSRCS       =./tests/*.c
INSTALLDIR     =~/Desktop
TEMP_DIR       =./tests/temp
REPORT_DIR_GCOV=./tests/report_gcov
REPORT_DIR_LCOV=./tests/report_lcov


ifeq ($(OS_NAME), Linux)
	TEST_FLAGS  =--coverage $(shell pkg-config --libs check)
	QMAKE = qmake6
else
	TEST_FLAGS  =--coverage -lcheck -lpthread -lm
	QMAKE = qmake
endif

all: install

install:
	rm -rf build
	mkdir build
	$(QMAKE) frontend/3DViewer.pro -o build
	cd build && make && make clean && cd ../

uninstall:
	rm -rf build

dvi:
	open doc.txt

dist: install
	tar -czf build.tar.gz --directory=build/ .

libmatrix.a: ./backend/my_matrix/*.c ./backend/my_matrix/*.h
	$(CC) $(CFLAGS) -c ./backend/my_matrix/*.c
	ar -rc $@ *.o
	ranlib $@
	rm -f *.o

.PHONY: tests
tests: clean libmatrix.a tests/*.[ch]
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(TESTSRCS) $(LIBSRCS) -L. -lmatrix -o test_viewer.out -lm `pkg-config --cflags --libs check`
	./test_viewer.out
	mkdir -p $(TEMP_DIR)
	mv *.gcda $(TEMP_DIR)
	mv *.gcno $(TEMP_DIR)

gcov_report: tests
	mkdir -p $(REPORT_DIR_LCOV)
	lcov -t ./test_viewer.out -o $(REPORT_DIR_LCOV)/t_lcov.info -c -d . --no-external
	genhtml -o $(REPORT_DIR_LCOV) $(REPORT_DIR_LCOV)/t_lcov.info
	open $(REPORT_DIR_LCOV)/index.html

clean:
	-rm -rf *.o *.html *.gcda *.gcno *.a *.gcov *.info *.out *.cfg gcov* \
	*.dSYM $(TEMP_DIR) $(REPORT_DIR_GCOV) $(REPORT_DIR_LCOV) build *.tar.gz

style_check:
	clang-format -style=Google -n */*.c */*.h
