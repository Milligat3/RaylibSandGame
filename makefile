COMPILER_PATH := D:/ProgrStuff/raylib/w64devkit/bin
CC := $(COMPILER_PATH)/gcc.exe
TARGET := Sand.exe
SRC_DIR := src
OBJ_DIR := obj
INCFLAGS := -Iinclude -ID:/ProgrStuff/raylib/raylib/src 
LDFLAGS := -LD:/ProgrStuff/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -mconsole

all: $(TARGET)

$(TARGET): obj/main.o obj/sand.o obj/figure.o
	$(CC) -g $^ -o $@ $(LDFLAGS)

obj/sand.o: src/sand.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -std=c99 $(INCFLAGS) -c $< -o $@

obj/figure.o: src/figure.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -std=c99 $(INCFLAGS) -c $< -o $@

obj/main.o: src/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -std=c99 $(INCFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	
love:
	@echo "Hey! Don't you fucking dare give up! You're the only soul that can help this world in this room!"