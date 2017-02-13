CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := obj/Cell.o \
obj/FileIO.o \
obj/Grid.o \
obj/Raster.o \
obj/Table.o \
obj/Vertex.o \
obj/main.o

LD_FLAGS := -L/usr/lib64/ -lGL -lGLU -lglut -lm -lstdc++
CC_FLAGS := -O2
# CC_FLAGS := -O2 -DMPICH_IGNORE_CXX_SEEK

bin/flush.exe: $(OBJ_FILES)
	gcc $(LD_FLAGS) $(OBJ_FILES) -o bin/GisToSWMM5.exe

obj/%.o: src/%.cpp
	@echo Compiling $<
	gcc $(CC_FLAGS) -c -o $@ $<
