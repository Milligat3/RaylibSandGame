// Figure.h
#pragma once
#include "sand.h"

#define INITIAL_OFFSET 10

#define REC_OFFSET 7
#define REC_SIZE 110

static const int fig_size = 128;
static const int block_size = 32;
static const int block_size_on_spawn = 24;
static const int max_figs = 3;

typedef struct
{
	int dx, dy;
}Block;

// БУ БЛЯТЬ! Put width & height to use, fatass. 
typedef struct
{
	Block fig[4];
	Sand fig_t;
	int x, y, x_fic, y_fic;
	float anchor_x, anchor_y;
	bool is_grabbed, is_grabable;
	int max_x, min_x, max_y, min_y;
	int width, height;
	int spawn_x, spawn_y;
}Figure;

extern Figure fig_table[9];

extern Figure current_figs[3];
extern int figure_count;

extern Sand **g_space;
extern Sand **g_back;

extern Sand col_table[4];
extern Sand current_color;

void InitFigures(void);
void DrawFigures(int ind);

int WhichFigure(Vector2 mouse);
void MoveFigure(Vector2 mouse, int fig_num);
bool UpdateFigures(void);
bool in_bounds(Figure fig);
bool TouchesSand(Figure fig);

void DrawUI(void);