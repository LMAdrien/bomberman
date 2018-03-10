#ifndef MAP_H
#define MAP_H

#include "../shared/shared.h"

void init_map(t_map* map);
void empty_place_player1(t_map* map);
void empty_place_player2(t_map* map);
void empty_place_player3(t_map* map);
void empty_place_player4(t_map* map);
void put_player_to_map(t_map* map, int player);

#endif