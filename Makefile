example: example.o serLCD.o
	gcc -Wall $^ -o $@ -li2c

%.o:	%.c %.h
	gcc -c -Wall -O2 $< -o $@

clean:
	rm -f *.o example
