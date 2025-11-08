// main.cpp
#include "raylib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sand.h"
#include "figure.h"

void DrawGSpace(RenderTexture2D* text)
{
	Color *pixels = malloc(s_size*s_size_h*sizeof(Color));
	for(int i = 0; i < s_size; i++)
	{
		for(int j = 0; j < s_size_h; j++)
		{
			Color col = g_space[i][j].col;
			pixels[i*s_size_h + j] = col;
		}
	}
	
	UpdateTextureRec(text->texture, (Rectangle){0, 0, s_size_h, s_size}, pixels);

	free(pixels);
}

typedef struct
{
	bool fading;
	bool changing;
	bool falling;
	bool is_paused;
	bool grab;
	int index;
}GameState;

GameState gs = {0};

void HandleMouse(void)
{
	
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		Vector2 mouse = GetMousePosition();
		if(!gs.grab){
			gs.index = WhichFigure(mouse);
			gs.grab = true;
		}
		MoveFigure(mouse, gs.index);
	}
	if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		for(int i = 0; i < 3; i++)
			current_figs[i].is_grabbed = false;
		if(gs.index >= 0 && gs.index < max_figs){

			current_figs[gs.index].is_grabable = false;

			if(!in_bounds(current_figs[gs.index]) || TouchesSand(current_figs[gs.index]))
			{
				current_figs[gs.index].is_grabable = true;
				current_figs[gs.index].x_fic = current_figs[gs.index].x;
				current_figs[gs.index].y_fic = current_figs[gs.index].y;
			}
		}
		gs.grab = false;
		gs.index = -1;
	}
}

void HandleKeyboard(void)
{
	if(IsKeyPressed(KEY_F5))
	{
		Init_Board();
		InitFigures();
	}
	if(IsKeyPressed(KEY_P))
	{
		gs.is_paused ^= 1;
	}
}

bool colEqu(Color c1, Color c2)
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

char* getColorStr(void)
{
	if(colEqu(current_color.col, RED))
		return "Red";
	if(colEqu(current_color.col, BLUE))
		return "Blu";
	if(colEqu(current_color.col, GREEN))
		return "Grn";
	if(colEqu(current_color.col, YELLOW))
		return "Ylw";
	return "Unreachable";
}

int main()
{
	srand(time(NULL));
	InitWindow(w_size_h_w_p, w_size_v, "Sand it is!");
	SetTargetFPS(0);
	Init_Board();
	InitFigures();
	RenderTexture2D text = LoadRenderTexture(s_size_h, s_size);
	SetTextureFilter(text.texture, TEXTURE_FILTER_POINT);
	int frame_counter = 0;
	clock_t start = clock();
	clock_t end = start;
	clock_t check_st = start;
	clock_t check_end = start;
	clock_t check_fade = start;
	clock_t check_fade_end = start;
	while(!WindowShouldClose())
	{
		end = clock();
		check_end = end;
		check_fade_end = end;
		HandleKeyboard();
		
		
		if(!gs.is_paused){
			if(!gs.fading)
			{
				HandleMouse();
				if(end - start >= 0.016 * CLOCKS_PER_SEC)
				{
					start = end;
					gs.changing = Update_sand();
					gs.falling = UpdateFigures();
				}
				if(figure_count == 0)
					InitFigures();
				if((check_end - check_st >= 1 * CLOCKS_PER_SEC) && !gs.changing && !gs.falling)
				{
					check_st = check_end;
					if(CheckLayers())
					{
						if(gs.index != -1)
						{
							current_figs[gs.index].is_grabbed = false;
							current_figs[gs.index].x_fic = current_figs[gs.index].x;
							current_figs[gs.index].y_fic = current_figs[gs.index].y;
							gs.index = -1;
						}
					}
				}
			}	
			if(check_fade_end - check_fade >= 0.02 * CLOCKS_PER_SEC)
			{
				check_fade = check_fade_end;
				gs.fading = Update_fade();
			}
		}
		DrawGSpace(&text);
		BeginDrawing();
		ClearBackground(BLACK);
		DrawUI();
		DrawTexturePro(text.texture, (Rectangle){0, 0, s_size_h, s_size}, (Rectangle){padding, padding, w_size_h, w_size},(Vector2){0, 0}, 0.0f, WHITE);
		DrawFigures(gs.index);
		DrawFPS(padding, 30);
		EndDrawing();

	}

	UnloadRenderTexture(text);
	CloseWindow();
	Cleanup();
	return 0;
}
