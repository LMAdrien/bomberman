#include "map.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void init_map(t_map* map)
{
    int x;
    int y;
    int r;
    printf("Rand\n");
    srand(time(NULL));
    for(x = 0; x < MAP_BORDER; x++)
    {
        for(y = 0; y < MAP_BORDER; y++)
        {
            r = rand();

            if(x == 0 || y == 0 || x == MAP_BORDER - 1 || y == MAP_BORDER - 1)
            {
                *map[x * MAP_BORDER + y] = 'B'; /*bordure*/
            }
            else if(x % 2 == 0 && y % 2 == 0)
            {
                *map[x * MAP_BORDER + y] = 'I'; /*case indestructible*/
            }
            else
            {
                if (r % 2 == 0)
                {
                    *map[x * MAP_BORDER + y] = 'E'; /*vide*/
                }
                else
                {
                    *map[x * MAP_BORDER + y] = 'D'; /*brique destructible*/
                }
            }
        }
    }
    empty_place_player1(map);
    empty_place_player2(map);
    empty_place_player3(map);
    empty_place_player4(map);
}

void empty_place_player1(t_map* map)
{
    *map[MAP_BORDER + 1] = 'E';
    *map[MAP_BORDER * 2 + 1] = 'E';
    *map[MAP_BORDER + 2] = 'E';
}

void empty_place_player2(t_map* map)
{
    *map[MAP_BORDER * (MAP_BORDER - 2) + 1] = 'E';
    *map[MAP_BORDER * (MAP_BORDER - 3) + 1] = 'E';
    *map[MAP_BORDER * (MAP_BORDER - 2) + 2] = 'E';
}

void empty_place_player3(t_map* map)
{
    *map[MAP_BORDER + (MAP_BORDER - 2)] = 'E';
    *map[MAP_BORDER * 2 + (MAP_BORDER - 2)] = 'E';
    *map[MAP_BORDER + (MAP_BORDER - 3)] = 'E';
}

void empty_place_player4(t_map* map)
{
    *map[MAP_BORDER * (MAP_BORDER - 2) + (MAP_BORDER - 2)] = 'E';
    *map[MAP_BORDER * (MAP_BORDER - 3) + (MAP_BORDER - 2)] = 'E';
    *map[MAP_BORDER * (MAP_BORDER - 2) + (MAP_BORDER - 3)] = 'E';
}

void put_player_to_map(t_map* map, int player)
{
    if (player == 0)
    {
        *map[MAP_BORDER + 1] = '1';
    }
    else if (player == 1)
    {
        *map[MAP_BORDER * (MAP_BORDER - 2) + 1] = '2';
    }
    else if (player == 2)
    {
        *map[MAP_BORDER + (MAP_BORDER - 2)] = '3';
    }
    else if (player == 3)
    {
        *map[MAP_BORDER * (MAP_BORDER - 2) + (MAP_BORDER - 2)] = '4';
    }
}