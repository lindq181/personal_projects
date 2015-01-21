# Program files.
utility.o:	utility.c
	gcc -c utility.c

#executable

lab3:	utility
	./utility

utility:	utility.o
	gcc -o utility utility.c

clean:
	rm -Rf *.o \
		utility	
