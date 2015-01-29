


build:
	gcc -Wall -L/usr/X11R6/lib  -lX11 clusterd.c -o clusterd 

clean:
	rm clusterd

