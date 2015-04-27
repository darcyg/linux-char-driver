CROSS=arm-linux-

all: test_eint

test_eint:test_eint.c
	$(CROSS)gcc test_eint.c -o test_eint
	$(CROSS)strip test_eint
clean:
	@rm -vf test_eint *.o *~
