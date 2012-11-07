# Makefile to build PORR1 program
# --- macros
CC=gcc -g
#CFLAGS=  -O3 -I /usr/local/lib/sprng/include -I /usr/local/lib/pgplot -g
OBJECTS=main.o evolution.o globals.o glut_impl.o roulette.o qsortPopulation.o evolutionLib.o
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

roulette.o: roulette.c
	$(CC) $(CFLAGS) -c roulette.c

qsortPopulation.o: qsortPopulation.c
	$(CC) $(CFLAGS) -c qsortPopulation.c 

evolutionLib.o: evolutionLib.c
	$(CC) $(CFLAGS) -c evolutionLib.c 

# --- remove binary and executable files
clean:
	rm -f porr1 $(OBJECTS)