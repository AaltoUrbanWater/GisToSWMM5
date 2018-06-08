# Makefile for GisToSWMM5

EXEC = bin/GisToSWMM5
CPP_FILES = $(wildcard src/*.cpp)
ODIR = obj
OBJ_FILES = $(patsubst src/%.cpp,$(ODIR)/%.o,$(CPP_FILES))
LD_FLAGS = -L/usr/lib/
CC_FLAGS = -O2 -DNDEBUG -std=c++11
CC = g++


$(EXEC) : $(OBJ_FILES)
	$(CC) $(LD_FLAGS) $(CC_FLAGS) -o $@ $^

$(ODIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c -o $@ $<

.PHONY : clean
clean:
	rm -f $(ODIR)/*.o
	rm -f *~ 
	rm -f $(EXEC)
