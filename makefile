


build:
	gcc -Wall -L/usr/X11R6/lib -ludev -lX11 clusterd.c -o clusterd 

debug:
	gcc -Wall -g -L/usr/X11R6/lib -ludev -lX11 clusterd.c -o clusterd

clean:
	rm clusterd

