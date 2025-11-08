// figure.c
#include "figure.h"
#include "raylib.h"
#include "sand.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

Figure fig_table[9] = { (Figure){.fig={{ 0,  0}, { 1,  0}, {-1,  0}, {-2,  0}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.0f, .anchor_y=-0.5f, .is_grabbed=false, .is_grabable=true, .width=4, .height=1}, // |
						(Figure){.fig={{ 0,  0}, {-1,  0}, { 0,  1}, {-1,  1}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.0f, .anchor_y=-1.0f, .is_grabbed=false, .is_grabable=true, .width=2, .height=2}, // O
						(Figure){.fig={{ 0,  0}, {-1,  0}, { 1,  0}, { 1, -1}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.5f, .anchor_y=-0.5f, .is_grabbed=false, .is_grabable=true, .width=3, .height=2}, // L
						(Figure){.fig={{ 0,  0}, {-1,  0}, { 1,  0}, {-1, -1}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.5f, .anchor_y=-0.5f, .is_grabbed=false, .is_grabable=true, .width=3, .height=2}, // L
						(Figure){.fig={{ 0,  0}, {-1,  0}, { 1,  0}, { 0,  1}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.5f, .anchor_y=-0.5f, .is_grabbed=false, .is_grabable=true, .width=3, .height=2}, // T
						(Figure){.fig={{ 0,  0}, { 1,  0}, {-1,  0}, { 0, -1}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.5f, .anchor_y=-0.5f, .is_grabbed=false, .is_grabable=true, .width=3, .height=2}, // T
						(Figure){.fig={{ 0,  0}, {-1,  0}, { 0,  1}, { 1,  1}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.5f, .anchor_y=-1.0f, .is_grabbed=false, .is_grabable=true, .width=3, .height=2}, // S
						(Figure){.fig={{ 0,  0}, { 1,  0}, {-1,  1}, { 0,  1}}, .fig_t=EMPTY, .x=0, .y=0, .x_fic=0, .y_fic=0, .anchor_x=0.5f, .anchor_y=-1.0f, .is_grabbed=false, .is_grabable=true, .width=3, .height=2}, // S
						(Figure){.fig={{ 0,  0}, { 0,  0}, { 0,  0}, { 0,  0}}, .fig_t=EMPTY, .x=-100, .y=-100, .x_fic=-100, .y_fic=-100, .is_grabbed=false, .is_grabable=false, .width=0, .height=0}};

Figure current_figs[3] = {0};

int figure_count = 0;

void calc_max(void)
{
	for(int i = 0; i < 3; i++){
		current_figs[i].min_x = current_figs[i].fig[0].dx;
		current_figs[i].max_x = current_figs[i].fig[0].dx;
		current_figs[i].min_y = current_figs[i].fig[0].dy;
		current_figs[i].max_y = current_figs[i].fig[0].dy;
		for(int j = 1; j < 4; j++)
		{
			if(current_figs[i].min_x > current_figs[i].fig[j].dx) current_figs[i].min_x = current_figs[i].fig[j].dx;
			if(current_figs[i].max_x < current_figs[i].fig[j].dx) current_figs[i].max_x = current_figs[i].fig[j].dx;
			if(current_figs[i].min_y > current_figs[i].fig[j].dy) current_figs[i].min_y = current_figs[i].fig[j].dy;
			if(current_figs[i].max_y < current_figs[i].fig[j].dy) current_figs[i].max_y = current_figs[i].fig[j].dy;
		}
	}
}



void InitFigures(void)
{
	figure_count = max_figs;
	for(int i = 0; i < 3; i++)
	{
		current_figs[i] = fig_table[rand() % 8];
		current_figs[i].fig_t = col_table[rand() % 4];
		current_figs[i].x = i*(117) + ((float)117)/2 - current_figs[i].anchor_x*block_size_on_spawn;
		current_figs[i].y = w_size_v-(block_size*4);
		current_figs[i].x_fic = current_figs[i].x;
		current_figs[i].y_fic = current_figs[i].y;
		current_figs[i].spawn_x = current_figs[i].x;
		current_figs[i].spawn_y = current_figs[i].y;
	}
	calc_max();
}

void DrawFigure(Figure fig)
{
	
	for(int i = 0; i < 4; i++)
	{
		if(fig.x_fic != fig.spawn_x || fig.y_fic != fig.spawn_y)
		{
			DrawRectangle(fig.x_fic + fig.fig[i].dx*block_size, fig.y_fic + fig.fig[i].dy*block_size, block_size, block_size, fig.fig_t.col);
		}
		else
		{
			DrawRectangle(fig.x_fic + fig.fig[i].dx*block_size_on_spawn, fig.y_fic + fig.fig[i].dy*block_size_on_spawn, block_size_on_spawn, block_size_on_spawn, fig.fig_t.col);
		}
	}
	DrawCircleV((Vector2){fig.x_fic+fig.anchor_x*block_size, fig.y_fic-fig.anchor_y*block_size}, 10, RED);
}

void DrawUI(void)
{
	for(int i = 0; i < 3; i++)
	{
		DrawRectangle(2*i*REC_OFFSET + REC_OFFSET + i*REC_SIZE, w_size_v - (5*block_size), REC_SIZE, REC_SIZE, (Color){0, 30, 30, 255});
	}
}

void DrawFigures(int ind)
{
	for(int i = 0; i < 3; i++)
	{
		DrawFigure(current_figs[i]);
	}
	if(ind != -1)
		DrawFigure(current_figs[ind]);	
}

int WhichFigure(Vector2 mouse)
{
	float x = mouse.x;
	float y = mouse.y;
	bool any_grabbed = false;
	for(int i = 0; i < 3; i++)
		any_grabbed = current_figs[i].is_grabbed ? true : any_grabbed;
	for(int i = 0; i < 3; i++)
	{
		int bound_r_x = current_figs[i].x_fic + (current_figs[i].max_x+1)*block_size;
		int bound_l_x = current_figs[i].x_fic + current_figs[i].min_x*block_size;
		int bound_r_y = current_figs[i].y_fic + (current_figs[i].max_y+1)*block_size;
		int bound_l_y = current_figs[i].y_fic + current_figs[i].min_y*block_size;
		if((x >= bound_l_x && x < bound_r_x && y >= bound_l_y && y < bound_r_y && current_figs[i].is_grabable) || current_figs[i].is_grabbed)
		{
			current_figs[i].is_grabbed = true;
			return i;
		}
	}
	return -1;
}

void MoveFigure(Vector2 mouse, int fig_num)
{
	Figure cur_fig = current_figs[fig_num];
	float new_fic_x = mouse.x - cur_fig.anchor_x*block_size;
	float new_fic_y = mouse.y + cur_fig.anchor_y*block_size;
	float r_bound = new_fic_x + cur_fig.min_x*block_size;
	float l_bound = new_fic_x + (cur_fig.max_x+1)*block_size;
	float t_bound = new_fic_y + cur_fig.min_y*block_size;
	float d_bound = new_fic_y + (cur_fig.max_y+1)*block_size;
	if(r_bound > padding && l_bound < w_size_h_w_p - padding) 
	{	
		current_figs[fig_num].x_fic = new_fic_x;
	}
	else if(r_bound < padding)
	{
		current_figs[fig_num].x_fic = cur_fig.width/2*block_size + padding;
	}
	else if(l_bound > w_size_h_w_p - padding)
	{
		current_figs[fig_num].x_fic = w_size_h + padding - ((float)cur_fig.width/2+cur_fig.anchor_x)*block_size;
	}
	if(d_bound < w_size_v && t_bound > padding)
	{
		current_figs[fig_num].y_fic = new_fic_y;
	}
}


bool in_bounds(Figure fig)
{
	int x_fic = fig.x_fic;
	int y_fic = fig.y_fic;
	int left = x_fic + fig.min_x*block_size - padding;
	int right = x_fic + (fig.max_x+1)*block_size - padding;
	int top = y_fic + fig.min_y*block_size - padding;
	int bottom = y_fic + (fig.max_y+1)*block_size - padding;
	if(left < 0 ||  right > w_size_h) return false;
	if(top < 0  || bottom > w_size) return false;

	return true;
}

bool TouchesSand(Figure fig)
{
	int gridYmax = (fig.y_fic + (fig.max_y+1)*block_size - padding)/CELL_SIZE;
	
	if(gridYmax >= s_size - 1)
		return true;
	for(int i = 0; i < 4; i++)
	{
		int gridX = (fig.x_fic + fig.fig[i].dx*block_size - padding)/CELL_SIZE;
		int gridY = (fig.y_fic + (fig.fig[i].dy+1)*block_size - padding)/CELL_SIZE;
		for(int j = 0; j < 8; j++)
		{
			if(gridX+j >= s_size_h) continue;
			if(g_space[gridY][gridX+j].type != BLACK_S)
			{
				return true;
			} 
		}
	}

	return false; 
}

void ConvertToSand(Figure fig)
{
	int gridX = (fig.x - padding)/CELL_SIZE;
	int gridY = (fig.y - padding)/CELL_SIZE;
	for(int k = 0; k < 4; k++)
	{
		int current_gX = gridX + fig.fig[k].dx*block_size/CELL_SIZE;
		int current_gY = gridY + fig.fig[k].dy*block_size/CELL_SIZE;
		for(int i = 0; i < 8; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				Sand fig_t_cur = fig.fig_t;
				fig_t_cur.col.r += rand() % 20 - 10;
				fig_t_cur.col.g += rand() % 20 - 10;
				fig_t_cur.col.b += rand() % 20 - 10;
				g_space[current_gY+i][current_gX+j] = fig_t_cur;
			}
		}
	}
}

bool UpdateFigures(void)
{
	bool falling = false;
	for(int i = 0; i < 3; i++)
	{
		if(!in_bounds(current_figs[i]) || current_figs[i].is_grabbed)
			continue;
		current_figs[i].x = current_figs[i].x_fic;
		current_figs[i].y = current_figs[i].y_fic;
		if(TouchesSand(current_figs[i]))
		{
			ConvertToSand(current_figs[i]);
			current_figs[i] = fig_table[8];
			figure_count--;
			falling = true;
		}
		else
		{
			current_figs[i].y += CELL_SIZE;
			current_figs[i].y_fic = current_figs[i].y;
			falling = true;
		}
	}
	return falling;
}