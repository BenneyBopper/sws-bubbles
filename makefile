OS=mac
#OS=windows

ifeq ($(OS), windows)
	SYSCFLAGS=-IC:\MinGW\include
	SYSLDFLAGS=-LC:\MinGW\lib -L.\ -lopengl32 -lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid
	RM=del
	CC=g++
	EXE=.exe
else ifeq ($(OS), mac)
	SYSCFLAGS=-I/opt/local/include
	SYSLDFLAGS=-L/opt/local/lib -lSDL2 -framework opengl
	RM=rm -f
	CC=g++
	EXE=.out
endif

CFLAGS=$(SYSCFLAGS) -Wall -std=c++11
LDFLAGS=$(SYSLDFLAGS) 

OBJS=main.o display.o sim.o
EXECUTABLE=a$(EXE)

$(EXECUTABLE):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<
