all:
	gcc main.c -o mymy -lm
	./mymy

clean:
	rm *.o 
