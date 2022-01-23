# Multi_verse DeliveryService
# Gavin Zimmerman


# Makefile

# Source
SRC_MAIN = main.c 
SRC_SYS = camera.c 
SRC_MVRE = objects.c render.c utils.c signal.c
SRC_VRS = multiverse.c simple.c

HDR = MvRE.h objects.h render.h main.h camera.h simple.h
EXE = Mv_DS0

all: $(EXE)
SRC = $(SRC_MAIN) $(SRC_SYS) $(SRC_MVRE) $(SRC_VRS)
SRCO = $(SRC:.c=.o)
CC = gcc
DFLG = -DDEVMODE



#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLG = -O3 -Wall -DUSEGLEW -DGLFW
LIBS = -lglfw3 -lglew32 -lglu32 -lopengl32 -lm
CLEAN = rm -f *.exe *.o *.a
else

#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG = -O3 -Wall -Wno-deprecated-declarations -DGLFW
LIBS = -lglfw -framework OpenGL -framework IOKit

#  Linux/Unix/Solaris
else
CFLG = -O3 -Wall -DGLFW
LIBS = -lglfw -lGL -lm
endif

#  OSX/Linux/Unix/Solaris
CLEAN = rm -f $(EXE) *.o *.a
endif


# Compilation / Linking
$(EXE): $(SRCO)
	$(CC) $(CFLG) $(DFLG) -o $@ $^ $(LIBS)

%.o: %.c $(HDR)
	$(CC) $(CFLG) $(DFLG) -c $< -o $@

clean:
	$(CLEAN)