CC := gcc

SRC_DIR := src
OBJ_DIR := obj

ifeq ($(OS),Windows_NT)
	TARGET := Sand.exe
	INCFLAGS := -Iinclude -ID:/ProgrStuff/raylib/raylib/src 
	LDFLAGS := -LD:/ProgrStuff/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -mconsole
else
	TARGET := Sand
	INCFLAGS := -Iinclude
    LDFLAGS := -lraylib -lm
endif
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
