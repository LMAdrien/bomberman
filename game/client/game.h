#ifndef __GAME_H__
#define __GAME_H__

#include "common.h"

void game_loop(int sock);
int draw_map(char* map, t_game_ressources *ressources, char* fps);
void init_ressources(t_game_ressources *ressources);
void wait_for_players(int player_count);
int check_if_win(int player_count);

#endif