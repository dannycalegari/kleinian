CC=g++
CFLAGS=-g -Wall -O3 -fast
IFLAGS=-I/usr/X11R6/include
LFLAGS=-L/usr/X11R6/lib -lX11
FRAME=-framework GLUT -framework Cocoa -framework OpenGL
all: kleinian

kleinian: kleinian.cc algebra.cc sort.cc kleinian_group.cc generate.cc glut_functions.cc graphics.cc draw.cc user_interface.cc read_write.cc
	$(CC) $(CFLAGS) $(IFLAGS) -o kleinian kleinian.cc $(FRAME) $(LFLAGS) -lm

clean: rm kleinian
