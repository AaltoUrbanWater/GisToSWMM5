# Makefile for ExtractDataSets

EXEC = bin/ExtractDataSets
CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(patsubst src/%.cpp,obj/%.o,$(CPP_FILES))
LD_FLAGS = -L/usr/lib/ -fopenmp
CC_FLAGS = -O2 -fopenmp
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
