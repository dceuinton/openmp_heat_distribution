# Makefile for 159.735 Assignment 3
#
# To build just type:
#
# make heat_demo

CPP = g++

# Flags for Mac OSX
#CFLAGS = -w -O3 -arch ppc -arch i386

# FLAGS for Linux
CFLAGS = -w -O3 -fopenmp

# Locally compiled modules
OBJS = fitsfile.o

# Link to CFITSIO libraries - modify these paths accordingly
LIBP = -L/home/s14026002/Documents/parallel/cfitsio/cfitsio
INCP = -I/home/s14026002/Documents/parallel/cfitsio/cfitsio

LIBS = -lcfitsio -lm

MODS = $(INCP) $(LIBP) $(LIBS) $(OBJS) 

BINS = heat_demo

all : $(BINS)

clean :
	rm -f $(BINS)
	rm -f *.o

# Demo program. Add more programs by making entries similar to this
heat_demo : heat_demo.cpp draw.hxx array.hxx $(OBJS)
	${CPP} $(CFLAGS) -o heat_demo heat_demo.cpp $(MODS)

# Modules compiled and linked separately
fitsfile.o : fitsfile.cpp fitsfile.h
	${CPP} $(CFLAGS) $(INCP) -c fitsfile.cpp

heat : heat.cpp draw.hxx array.hxx $(OBJS)
	${CPP} $(CFLAGS) -o heat heat.cpp $(MODS)

p : parallel_heat.cpp draw.hxx array.hxx $(OBJS)
	${CPP} $(CFLAGS) -o main parallel_heat.cpp $(MODS)
