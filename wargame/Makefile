EXE = aimp2
OBJS = aimp2.o minimax.o board.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall
LINKER = g++
LINKER_OPTS =

all: $(EXE) 

$(EXE): $(OBJS)
	$(LINKER) $(OBJS) $(LINKER_OPTS) -o $(EXE)
	
aimp2.o: aimp2.cpp minimax.cpp minimax.h board.cpp board.h
	$(COMPILER) $(COMPILER_OPTS) aimp2.cpp
	
minimax.o: minimax.cpp minimax.h board.cpp board.h
	$(COMPILER) $(COMPILER_OPTS) minimax.cpp
	
board.o: board.cpp board.h
	$(COMPILER) $(COMPILER_OPTS) board.cpp

clean:
	rm -f aimp2 *.o
