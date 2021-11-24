# Multiverse Delivery Service Makefile
# Gavin Zimmerman

# Source
SRC = panel.c signal.c camera.c verse0.c
HDR = game.h signal.h camera.h verse0.h linmath.h
EXE = Mv-DS

all: $(EXE)
SRCO = $(SRC:.c=.o)
CC = gcc

DFLG = -DDEV_MODE

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