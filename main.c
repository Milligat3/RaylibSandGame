// main.cpp
#include "SDL3/SDL.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_render.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELL_SIZE 8
#define EMPTY (Sand){(Color){0, 0, 0, 255}, BLACK}
#define s_size 32

const int w_size = 256;
void DrawRectangle(SDL_Renderer* ren, SDL_FRect rect) {
    SDL_RenderFillRect(ren, &rect);
}

typedef struct
{
	uint8_t r, g, b, a;
}Color;


typedef enum
{
	BLACK,
	RED,
	GREEN,
	BLUE,
	YELLOW
}sand_t;

typedef struct
{
	Color col;
	sand_t type;
}Sand;

Sand g_space[s_size*s_size];

void DrawRectInBuf(uint32_t* arr, Color col, int i, int j)
{
	for(int y = 0; y < CELL_SIZE; y++)
	{
		for(int x = 0; x < CELL_SIZE; x++)
		{
			arr[(i+y)*w_size + j+x] = (col.r << 24) | (col.g << 16) | (col.b << 8) | col.a;
		}
	}
}

void DrawGSpace(SDL_Renderer *rend, SDL_Texture *text)
{
	uint32_t pixel_sheet[w_size*w_size];
	memset(pixel_sheet, 0, w_size*w_size*sizeof(uint32_t));
	SDL_UpdateTexture(text, NULL, pixel_sheet, w_size*(sizeof(uint32_t)));
	for(int i = 0; i < s_size; i++)
	{
		for(int j = 0; j < s_size; j++)
		{
			DrawRectInBuf(pixel_sheet, g_space[i*s_size + j].col, i*CELL_SIZE, j*CELL_SIZE);
			// SDL_SetRenderDrawColor(rend, g_space[i*32+j].col.r, g_space[i*32+j].col.g, g_space[i*32+j].col.b, g_space[i*32+j].col.a);
			// DrawRectangle(rend, (SDL_FRect){i*8, j*8, 8, 8});
		}
	}
	SDL_UpdateTexture(text, NULL, pixel_sheet, w_size * sizeof(uint32_t));
	SDL_RenderTexture(rend, text, NULL, NULL);
	// SDL_RenderPresent(rend);
}

void Update_sand(void)
{
	Sand g_copy[s_size*s_size];
	memcpy(g_copy, g_space, s_size*s_size*sizeof(Sand));
	for(int i = 0; i < s_size; i++)
	{
		for(int j = 0; j < s_size; j++)
		{
			if(g_space[i*s_size + j].type != BLACK && g_space[(i+1)*s_size + j].type == BLACK && ((i+1)*s_size + j) < 1024)
			{
				g_copy[(i+1)*s_size + j] = g_space[i*s_size + j];
				g_copy[i*s_size + j] = EMPTY;
			}
			if(g_space[i*s_size + j].type != BLACK && ((i+1)*s_size + j) < 1024 && g_space[(i+1)*s_size + j].type != BLACK)
			{
				if(((i+1)*s_size + j+1) < 1024 && ((i+1)*s_size + j-1) < 1024 && g_space[(i+1)*s_size + j+1].type == BLACK && g_space[(i+1)*s_size + j-1].type == BLACK)
				{
					uint32_t random = rand();
					if((random & 0x1) == 0)
					{
						g_copy[(i+1)*s_size + j+1] = g_space[i*s_size + j];
					}
					else
					{
						g_copy[(i+1)*s_size + j-1] = g_space[i*s_size + j];
					}
					g_copy[i*s_size + j] = EMPTY;
				}
				else if(((i+1)*s_size + j-1) < 1024 && g_space[(i+1)*s_size + j-1].type == BLACK)
				{
					g_copy[(i+1)*s_size + j-1] = g_space[i*s_size + j];
					g_copy[i*s_size + j] = EMPTY;
				}
				else if(((i+1)*s_size + j+1) < 1024 && g_space[(i+1)*s_size + j+1].type == BLACK)
				{
					g_copy[(i+1)*s_size + j+1] = g_space[i*s_size + j];
					g_copy[i*s_size + j] = EMPTY;
				}
			}
		}
	}
	memcpy(g_space, g_copy, s_size*s_size*sizeof(Sand));
}

void Init_Board(void)
{
	for(int i = 0; i < s_size*s_size; i++)
	{
		g_space[i] = EMPTY;
	}
}

void HandleMouse(SDL_Event *event)
{
	
	int gridY = event->button.x / CELL_SIZE;
   	int gridX = event->button.y / CELL_SIZE;
   	if(gridX >= 0 && gridX < s_size && gridY >= 0 && gridY < s_size)
   	{
    	if(event->button.button == SDL_BUTTON_LEFT)
		{
			g_space[gridX*s_size + gridY] = (Sand){(Color){255, 0, 255, 255}, BLUE};
		}		
	}
}


int main()
{
	if(SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Sand it is", w_size, w_size, 0);
	if(!window)
	{
		fprintf(stderr, "Couldn't create a window. %s.\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
	SDL_Renderer* rend =  SDL_CreateRenderer(window, NULL);
	if(!rend)
	{
		fprintf(stderr, "Couldn't create a renderer. %s.\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
	
	SDL_SetRenderDrawColor(rend, 0, 15, 15, 255);
	SDL_RenderClear(rend);
	SDL_RenderPresent(rend);
	SDL_Texture* texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w_size, w_size);                                                                                                                                        

	Init_Board();
	uint8_t random = rand() / RAND_MAX;
	uint8_t r = (sinf((rand()%32)*0.1f + (rand()%32)*0.1f) * random * 0.5f + 0.5f) * 255;
	uint8_t g = (cosf((rand()%32)*0.1f + (rand()%32)*0.1f) * 0.5f + 0.5f) * 255;
	g_space[15] = (Sand){(Color){r, g, r, 255}, GREEN};
	const bool *keyb_st = SDL_GetKeyboardState(NULL);
	int running = 1;
	while(running)
	{
		SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) 
            {
            	case SDL_EVENT_QUIT: running = 0; break;
            	case SDL_EVENT_MOUSE_BUTTON_DOWN: HandleMouse(&event); break;
         	}

        }
		if(keyb_st[SDL_SCANCODE_ESCAPE])
		{
			running = 0;
		}
		Update_sand();
		// for(int i = 0; i < 32; i++)
		// {
		// 	for(int j = 0; j < 32; j++)
		// 	{
		// 		uint8_t random = rand() / RAND_MAX;
		// 		static float intensity = 0.0f;
		// 		intensity += 0.01f;
		// 		uint8_t r = (sinf(intensity + i*0.1f + j*0.1f)*sinf(intensity + i*0.1f + j*0.1f) * random * 0.5f + 0.5f) * 255;
		// 		uint8_t g = (cosf(intensity + i*0.1f + j*0.1f) * 0.5f + 0.5f) * 255;
		// 		g_space[i*32 + j] = (Sand){(Color){r, g, r, 255}, (sand_t) rand() % 5};
		// 	}
		// }
		SDL_SetRenderDrawColor(rend, 0, 15, 15, 255);
		
		SDL_RenderClear(rend);
		DrawGSpace(rend, texture);
		SDL_RenderPresent(rend);

		SDL_Delay(100);
	}

	SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}
