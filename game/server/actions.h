#ifndef ACTIONS_H
#define ACTIONS_H

#include "../shared/shared.h"

void moveLeft(t_map *map, char player);
void moveRight(t_map *map, char player);
void moveTop(t_map *map, char player);
void moveBottom(t_map *map, char player);
void putBomb(t_map *map, char player, t_bomb *bombs);
int getIndexPlayer(t_map *map, char player);
void setPlayerCase(t_map *map, char player, int index);

#endif