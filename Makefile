# Multi_verse DeliveryService
# Gavin Zimmerman


# Makefile

# Source
SRC_MAIN = main.c 
SRC_SYS = signal.c camera.c render.c objects.c multiverse.c
SRC_VRS = home.c

HDR = game.h signal.h camera.h render.h objects.h linmath.h home.h multiverse.h
EXE = Mv-DS

all: $(EXE)
SRCO = $(SRC_MAIN:.c=.o) $(SRC_SYS:.c=.o) $(SRC_VRS:.c=.o)
CC = gcc

DFLG = -DDEVMODE


#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW -DGLFW
LIBS=-lglfw3 -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
CFLG=-O3 -Wall -Wno-deprecated-declarations -DGLFW -DGLFW_USE_RETINA=0 -DRES=$(RES)
LIBS=-lglfw -framework OpenGL -framework IOKit

#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall -DGLFW
LIBS=-lglfw -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif


# Compilation / Linking
$(EXE): $(SRCO)
	$(CC) $(CFLG) $(DFLG) -o $(EXE) $(SRCO) $(LIBS)

%.o: %.c $(HDR)
	$(CC) $(CFLG) $(DFLG) -c $< -o $@

clean:
	$(CLEAN)