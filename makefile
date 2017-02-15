# Makefile for GisToSWMM5

EXEC = bin/GisToSWMM5
CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(patsubst src/%.cpp,obj/%.o,$(CPP_FILES))
LD_FLAGS = -L/usr/lib/
CC_FLAGS = -O2
CC = g++


$(EXEC) : $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

.PHONY : clean
clean:
	rm -f obj/*.o
	rm -f *~ 
	rm -f $(EXEC)
