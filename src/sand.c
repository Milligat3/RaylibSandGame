// Sand.c
#include "sand.h"
#include "raylib.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

int col_counter = 1;
Sand col_table[4] = {(Sand){RED_C, RED_S, 0}, (Sand){GREEN_C, GREEN_S, 0}, (Sand){BLUE_C, BLUE_S, 0}, (Sand){YELLOW_C, YELLOW_S, 0}};
Sand **g_space = NULL;
Sand **g_back = NULL;
Sand current_color;


void clear_buf(void)
{
	for(int i = 0; i < s_size; i++)
	{
		for(int j = 0; j < s_size_h; j++)
		{
			g_back[i][j] = EMPTY;
		}
	}
}

bool Update_sand(void)
{
	bool changed_any = false;
	const SandMove move_table[2][3] =  {{MOVE_DOWN, MOVE_DOWN_RIGHT, MOVE_DOWN_LEFT}, {MOVE_DOWN, MOVE_DOWN_LEFT, MOVE_DOWN_RIGHT}};
	for(int i = s_size-1; i >= 0; i--)
	{
		for(int j = 0; j < s_size_h; j++)
		{
			
			
			if(g_space[i][j].type == BLACK_S) continue;
			if(i >= s_size-1) {g_back[i][j] = g_space[i][j]; continue;}
			int variant = rand() % 2;
			SandMove move = MOVE_NONE;

			for(int d = 0; d < 3; d++)
			{
				SandMove test_move = move_table[variant][d];
				int di = i+1;
				int dj = j;
				switch (test_move) 
				{
					case MOVE_DOWN: break;
					case MOVE_DOWN_LEFT: if(j > 0) dj = j - 1; else continue; break;
					case MOVE_DOWN_RIGHT: if(j < s_size_h-1) dj = j + 1; else continue; break;
					default: continue; 
				}
				if(g_back[di][dj].type == BLACK_S)
				{
					move = test_move;
					break;
				}
			}
			if(move != MOVE_NONE)
			{
				int dj = j;
				switch (move) 
				{
					case MOVE_DOWN_LEFT: dj = j - 1; break;
					case MOVE_DOWN_RIGHT: dj = j + 1; break;
					default: break;
				}
				int random = rand() % 100;
				if(random > 70)
				{
					g_back[i][j] = g_space[i][j];
					changed_any = true;
					continue;		
				}
				g_back[i+1][dj] = g_space[i][j];
				changed_any = true;
			}
			else
			{
				g_back[i][j] = g_space[i][j];
			}	
			
		}
	}
	Sand** temp = g_space;
	g_space = g_back;
	g_back = temp;
	clear_buf();
	return changed_any;
}

void Cleanup(void)
{
	if(g_space)
	{
		for(int i = 0; i < s_size; i++)
		{
			free(g_space[i]);
		}
		free(g_space);
	}
	if(g_back)
	{
		for(int i = 0; i < s_size; i++)
		{
			free(g_back[i]);
		}
		free(g_back);
	}
}


void Init_Board(void)
{
	Cleanup();
	g_space = malloc(s_size*sizeof(Sand*));
	g_back = malloc(s_size*sizeof(Sand*));
	for(int i = 0; i < s_size; i++)
	{
		g_space[i] = malloc(s_size_h*sizeof(Sand));
		g_back[i] = malloc(s_size_h*sizeof(Sand));
		for(int j = 0; j < s_size_h; j++)
		{
			g_space[i][j] = EMPTY;
			g_back[i][j] = EMPTY;
		}
	}
	
}


void init_queue(Queue_t* que)
{
	que->nei = malloc(sizeof(Neigh_t) * MAX_QUEUE_SIZE);
	que->front = 0;
	que->rear = 0;
}

void free_queue(Queue_t* que)
{
	free(que->nei);
}

void queue_push(Queue_t *que, int i, int j)
{
	if ((que->rear + 1) % MAX_QUEUE_SIZE == que->front) return;
	que->nei[que->rear] = (Neigh_t){j, i};
	que->rear = (que->rear+1) % MAX_QUEUE_SIZE;
	
}

bool queue_empty(Queue_t *que)
{
	return que->front == que->rear;
}

Neigh_t queue_pop(Queue_t *que)
{
	Neigh_t cur = que->nei[que->front];
	que->front = (que->front + 1) % MAX_QUEUE_SIZE;
	return cur;
}

static const int neigh_to_check[8][2]= {{-1, -1},{-1, 0},{-1, 1},{0, 1},
										{1, 1}, {1, 0}, {1, -1}, {0, -1}};

bool CheckLayers(void)
{
	Queue_t queue;
	init_queue(&queue);
	
	bool found_layer = false;
	bool visited[s_size][s_size_h] = {0};
	sand_t checked_type;
	bool touches_right = false;
	int group_size = 0;
	Neigh_t *group = malloc(s_size*s_size_h*sizeof(Neigh_t));
	for(int i = 0; i < s_size; i++)
	{
		Sand current = g_space[i][0];
		if(current.type == BLACK_S || visited[i][0])
			continue;
		checked_type = g_space[i][0].type;
		queue_push(&queue, i, 0);
		visited[i][0] = true;
		group[group_size++] = (Neigh_t){0, i};
		while(!queue_empty(&queue))
		{
			Neigh_t cur = queue_pop(&queue);
			
			if(cur.x == s_size_h - 1)
				touches_right = true;
			for(int j = 0; j < 8; j++)
			{
				int nj = cur.x + neigh_to_check[j][1];
				int ni = cur.y + neigh_to_check[j][0];
				if(ni == s_size || nj == s_size_h || ni < 0 || nj < 0)
					continue;
				if(g_space[ni][nj].type == checked_type && !visited[ni][nj])
				{
					visited[ni][nj] = true;
					queue_push(&queue, ni, nj);
					group[group_size++] = (Neigh_t){nj, ni};
				}
			}
		}
		
		if(touches_right)
		{
			found_layer = true;
			for(int i = 0; i < group_size; i++)
			{
				int x = group[i].x;
				int y = group[i].y;
				g_space[y][x].fade_state = 30;
			}
			group_size = 0;
			
			break;
		}
		group_size = 0;
		
	}
	free(group);
	free_queue(&queue);
	return found_layer;
}

bool Update_fade(void)
{
	bool changed = false;
    for(int i = 0; i < s_size; i++)
    {
        for(int j = 0; j < s_size_h; j++)
        {
            // Только для ячеек в процессе удаления (1-5)
            if(g_space[i][j].fade_state > 0 && g_space[i][j].fade_state <= 30)
            {
            	changed = true;
                // Мерцание с инверсией цвета
                if(g_space[i][j].fade_state % 2 == 1) {
                    g_space[i][j].col = (Color){
                        255 - g_space[i][j].col.r,
                        255 - g_space[i][j].col.g,
                        255 - g_space[i][j].col.b,
                        255
                    };

                }
                
                // Уменьшаем стадию и удаляем в конце
                if(--g_space[i][j].fade_state == 0) {
                    g_space[i][j].type = BLACK_S;
                    g_space[i][j].col = (Color){0, 30, 30, 255};
                }
            }
        }
    }
    return changed;
}

