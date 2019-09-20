# HW2: Lorenz Attractor
# Course: CSCI 5229
# Author: Kathy Grimes
# Description:
#  This file is the gears Makefile from the first homework
#  Only name modifications have occurred (from gears to hw#)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f hw2 *.o *.a
endif

#  Compile and link
hw2:hw2.c
	gcc $(CFLG) -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
