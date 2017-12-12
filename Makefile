all: test

test: src/fpconv_test.c
	gcc -o fpconv_test $<
	./fpconv_test
