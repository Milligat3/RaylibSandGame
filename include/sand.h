// sand logic
#pragma once

#include "raylib.h"
#include <stdbool.h>

#define CELL_SIZE 4
#define s_size 128
#define s_size_h 88

#define RED_C        CLITERAL(Color){ 230, 41, 55, 255 }     // Red
#define GREEN_C      CLITERAL(Color){ 10, 228, 48, 255 }      // Green
#define BLUE_C       CLITERAL(Color){ 10, 121, 241, 255 }     // Blue
#define YELLOW_C     CLITERAL(Color){ 245, 245, 10, 255 }     // Yellow

static const int w_size = 512;
static const int w_size_h = 352;
static const int padding = 8;
static const int w_size_h_w_p = w_size_h + 2*padding;

static const int w_size_v = 720;


extern int col_counter;

#define EMPTY (Sand){(Color){0, 30, 30, 255}, BLACK_S, 0}
#define BORDER (Sand){BLACK, BLANK_S, 0}

typedef enum
{
	BLANK_S,
	BLACK_S,
	RED_S,
	GREEN_S,
	BLUE_S,
	YELLOW_S
}sand_t;

typedef struct
{
	Color col;
	sand_t type;
	int fade_state;
}Sand;

void clear_buf(void);

bool Update_sand(void);

void Cleanup(void);

void Init_Board(void);

bool CheckLayers(void);

bool Update_fade(void);

#define MAX_QUEUE_SIZE (s_size * s_size_h)

typedef struct
{
	int x, y;
}Neigh_t;

typedef struct
{
	Neigh_t* nei;
	int front, rear;
}Queue_t;

void init_queue(Queue_t* que);
void free_queue(Queue_t* que);
void queue_push(Queue_t* que, int i, int j);
Neigh_t queue_pop(Queue_t *que);
bool queue_empty(Queue_t* que);

typedef enum 
{
	MOVE_DOWN,
	MOVE_DOWN_RIGHT,
	MOVE_DOWN_LEFT,
	MOVE_NONE
}SandMove;