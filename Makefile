# Makefile to build PORR1 program
# --- macros
CC=gcc
#CFLAGS=  -O3 -I /usr/local/lib/sprng/include -I /usr/local/lib/pgplot -g
OBJECTS=main.o evolution.o globals.o glut_impl.o
LIBS = -lGL -lGLU -lglut -lm


# --- targets
all:    porr1
porr1:   $(OBJECTS) 
	$(CC)  -o porr1  $(OBJECTS) $(LIBS)
        
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
       
evolution.o:  evolution.c
	$(CC) $(CFLAGS) -c evolution.c 

globals.o: globals.c
	$(CC) $(CFLAGS) -c globals.c 

glut_impl.o: glut_impl.c
	$(CC) $(CFLAGS) -c glut_impl.c 

# --- remove binary and executable files
clean:
	rm -f porr1 $(OBJECTS)