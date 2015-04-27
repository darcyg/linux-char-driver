CROSS=arm-linux-

all: test_led

test_led:test_led.c
	$(CROSS)gcc -o test_led test_led.c
	$(CROSS)strip test_led
clean:
	@rm -vf test_led *.o *~
