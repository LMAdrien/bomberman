#include "actions.h"
#include <stdio.h>
#include <time.h>

void moveLeft(t_map *map, char player)
{
    int index;
    index = getIndexPlayer(map, player);
    if (*map[index - 1] == 'E' )
    {
        *map[index - 1] = player; 
        setPlayerCase(map, player, index);
    }
}

void moveRight(t_map *map, char player)
{
   int index;
   index = getIndexPlayer(map, player);
   if (*map[index + 1] == 'E' )
    {
        *map[index + 1] = player;
        setPlayerCase(map, player, index);
    }
}

void moveTop(t_map *map, char player)
{
   int index;
   index = getIndexPlayer(map, player);
   if (*map[index - 15] == 'E' )
    {
        *map[index - 15] = player; 
        setPlayerCase(map, player, index);
    }
}

void moveBottom(t_map *map, char player)
{
   int index;
   index = getIndexPlayer(map, player);
   if (*map[index + 15] == 'E')
    {
        *map[index + 15] = player; 
        setPlayerCase(map, player, index);
    }
}

void putBomb(t_map *map, char player, t_bomb *bombs)
{
   int index;
   index = getIndexPlayer(map, player);

   bombs[index].index = index; 
   bombs[index].power = 2; 
   bombs[index].start_time = 12; 
   
   if (*map[index] == player)
   {
       *map[index] = player + 4; 
   }
}

int getIndexPlayer(t_map *map, char player)
{
    int i;
    for (i = 0; i < MAP_SIZE; i++)
    {
        if (*map[i] == player || *map[i] == player + 4)
        {
            return (i);
        }
    }
    return (-1);
}

void setPlayerCase(t_map *map, char player, int index)
{
    if (*map[index] == player + 4)
    {
        *map[index] = 'T'; /*TNT*/
    }
    else if (*map[index] == player)
    {
        *map[index] = 'E'; /*EMPTY*/
    }
}