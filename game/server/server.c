#include "server.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include "map.h"
#include "../client/common.h"

void start_server()
{
	t_socket_params socket_params;
	t_game *game = malloc(sizeof(t_game));
	t_bomb *bombs = malloc(sizeof(t_bomb)* 225);
	t_bomb_destroyed *bombs_destroyed = malloc(sizeof(t_bomb_destroyed)* 225);
	
	clock_t start_time;
	clock_t update_time;
	float time_elapsed;
	int we_have_a_winner;

	game->map = malloc(sizeof(t_map));
	game->map_string = (char *) malloc(sizeof(char) * MAP_SIZE);
	init_map(game->map);

	/* Select timeout 5 sec */
    socket_params.tv.tv_sec = 0;
    socket_params.tv.tv_usec = 100;
	socket_params = init_socket(socket_params);

	start_time = clock();
	we_have_a_winner = 0;
	while(we_have_a_winner == 0)
	{
		update_time = clock();
		time_elapsed = ((float)(update_time - start_time) / 1000000.0F ) * 1000;
		if (time_elapsed >= 200.0F)
		{
			decrease_bomb(game, bombs, bombs_destroyed);
			decrease_bomb_destroyer(game, bombs_destroyed);
			start_time = update_time;
			socket_params = clear_socket(socket_params);
			/* Wait for activity */
 			socket_params.activity = select( socket_params.max_sd + 1 , &(socket_params.readfds), NULL, NULL, &(socket_params.tv));

			if ((socket_params.activity < 0) && (errno!=EINTR))
			{
				printf("select error");
			}
			socket_params = check_connexion(socket_params, game);
			socket_params = read_client_socket(socket_params, game, bombs);
			we_have_a_winner = check_if_win_server(game);
		}
	}
}

int check_if_win_server(t_game *game)
{
	int i;
	int player_count;
	player_count = 0;
	for(i = 0; i < MAP_SIZE; i++)
	{
		if (*game->map[i] == '1' ||*game->map[i] == '2' ||*game->map[i] == '3' ||*game->map[i] == '4' ||*game->map[i] == '5' ||*game->map[i] == '6' ||*game->map[i] == '7' ||*game->map[i] == '8')
		{
			player_count++;
		}
	}
    if (*game->map[0] == 'S' && player_count <= 1)
    {
        return (1);
    }
    return (0);
}

void send_map(t_socket_params socket_params, t_game *game)
{
	int j;

	j = 0;
	my_strcat(game);

	while(j < MAX_PLAYERS)
	{
		if(socket_params.client_socket[j] != 0 && (size_t)send(socket_params.client_socket[j], game->map_string, 225, 0) != 225 )
		{
			printf("Error sending message\n");
			perror("send map failed");
		}
		j++;
	}
}

t_socket_params check_connexion(t_socket_params socket_params, t_game *game)
{
	int i;
	char *connection_success_message;

	connection_success_message = "Connected to server\r\n";

	if (FD_ISSET(socket_params.parent_socket, &(socket_params.readfds)))
	{
		if ((socket_params.child_socket = accept(socket_params.parent_socket, (struct sockaddr *) &(socket_params.address), (socklen_t*) &(socket_params.addrlen))) < 0)
		{
			perror("accept failed");
			exit(EXIT_FAILURE);
		}
		printf("New connection\n");

		if( (size_t)send(socket_params.child_socket, connection_success_message, strlen(connection_success_message), 0) != strlen(connection_success_message) )
		{
			perror("send welcome message failed");
		}
		printf("Welcome message sent successfully\n");

		/* add new socket to array of sockets */
		for (i = 0; i < MAX_PLAYERS; i++)
		{
			if( socket_params.client_socket[i] == 0 )
			{
				socket_params.client_socket[i] = socket_params.child_socket;
				/*Add player to map*/
				put_player_to_map(game->map, i);
				i = MAX_PLAYERS;
			}
		}
		send_map(socket_params, game);
	}
	return (socket_params);
}

t_socket_params clear_socket(t_socket_params socket_params)
{
	int i;
	/* Clear the socket set */
	FD_ZERO(&(socket_params.readfds));

	/* add master socket to set */
	FD_SET(socket_params.parent_socket, &(socket_params.readfds));
	socket_params.max_sd = socket_params.parent_socket;
	/* add child sockets to set */
	for ( i = 0 ; i < MAX_PLAYERS ; i++)
	{
		socket_params.sd = socket_params.client_socket[i];
		if(socket_params.sd > 0)
		{
			FD_SET( socket_params.sd , &(socket_params.readfds));
		}
		if(socket_params.sd > socket_params.max_sd)
		{
			socket_params.max_sd = socket_params.sd;
		}
	}
	return (socket_params);
}

t_socket_params read_client_socket(t_socket_params socket_params, t_game *game, t_bomb *bombs)
{
	int i;
	
	for (i = 0; i < MAX_PLAYERS; i++)
	{
		socket_params.sd = socket_params.client_socket[i];

		if (FD_ISSET( socket_params.sd , &(socket_params.readfds)))
		{
			/* Check if connexion was closed */
			if ((recv(socket_params.sd, &(socket_params.buffer), 1, 0)) < 1)
			{
				/*Client disconnected */
				getpeername(socket_params.sd, (struct sockaddr*) &(socket_params.address), (socklen_t*) &(socket_params.addrlen));
				printf("Client disconnected\n");
				close(socket_params.sd);
				socket_params.client_socket[i] = 0;
			}
			else
			{
//				printf("Client %d say : %s\n", i + 1, socket_params.buffer); 
				if (strcmp(socket_params.buffer, "U") == 0)
				{
					moveTop(game->map, (char)(i + 49));
				}
				else if (strcmp(socket_params.buffer, "D") == 0)
				{
					moveBottom(game->map, (char)(i + 49));
				}
				else if(strcmp(socket_params.buffer, "L") == 0)
				{
					moveLeft(game->map, (char)(i + 49));
				}
				else if(strcmp(socket_params.buffer, "R") == 0)
				{
					moveRight(game->map, (char)(i + 49));
				}
				else if(strcmp(socket_params.buffer, "S") == 0)
				{
					putBomb(game->map, (char)(i + 49), bombs);
				}
				else if(strcmp(socket_params.buffer, "A") == 0)
				{
					*game->map[0] = 'S';
				}
				else
				{
					//No action reconized
				}
			}
		}
	}
	send_map(socket_params, game);

	return (socket_params);
}

t_socket_params set_socket_options(t_socket_params socket_params)
{
	/* set multiple connections */
	if( setsockopt(socket_params.parent_socket, SOL_SOCKET, SO_REUSEADDR, (struct timeval *) &(socket_params.tv), sizeof(struct timeval)) < 0 )
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

    socket_params.address.sin_family = AF_INET;
	socket_params.address.sin_addr.s_addr = INADDR_ANY;
	socket_params.address.sin_port = htons( PORT );

	return (socket_params);
}

t_socket_params init_socket(t_socket_params socket_params)
{
	int i;

	/* initialise all client to 0 */
	for (i = 0; i < MAX_PLAYERS; i++)
	{
		socket_params.client_socket[i] = 0;
	}

	/* create a parent socket */
	if( (socket_params.parent_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	socket_params = set_socket_options(socket_params);

	/* bind the socket */
	if (bind(socket_params.parent_socket, (struct sockaddr *) &(socket_params.address), sizeof(socket_params.address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Server is running on port: %d \n", PORT);

	/* pending connections */
	if (listen(socket_params.parent_socket, MAX_PLAYERS) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	socket_params.addrlen = sizeof(socket_params.address);
	printf("Waiting for connections ...\n");
	server_is_lunched = 1;
	return (socket_params);
}

void my_strcat(t_game *game)
{
	int i;
	for (i = 0; i < MAP_SIZE; i++)
	{
		game->map_string[i] = '\0';
	}
	for (i = 0; i < MAP_SIZE; i++)
	{
		game->map_string[i] = *(game->map)[i];
	}
}

void decrease_bomb(t_game* game, t_bomb* bombs, t_bomb_destroyed *bombs_destroyed)
{
	int i;
	int x;
	int y;
	
	for( i = 0; i < MAP_SIZE; i++)
	{
		if(*(game->map)[i] == 'T' || *(game->map)[i] == '5' || *(game->map)[i] == '6'|| *(game->map)[i] == '7' || *(game->map)[i] == '8')
		{
			bombs[i].start_time--;
			if (bombs[i].start_time <= 0)
			{
				//*(game->map)[i] = 'X';
				//printf("%d \n", bombs[i].start_time);
				for(x = 0; x <= bombs[i].power; x++)
				{
					if((x * MAP_BORDER) + i > 0)
					{
						if (*(game->map)[(x * MAP_BORDER) + i] == 'D')
						{
							*(game->map)[(x * MAP_BORDER) + i] = 'X';
							x = bombs[i].power + 1;
						}
						else if (*(game->map)[(x * MAP_BORDER) + i] == 'I' || *(game->map)[(x * MAP_BORDER) + i] == 'B')
						{
							x = bombs[i].power + 1;
						}
						else
						{
							*(game->map)[(x * MAP_BORDER) + i] = 'X';
						}
					}
				}

				for(x = 0; x >= (bombs[i].power) * -1; x--)
				{
					if((x * MAP_BORDER) + i > 0)
					{
						if (*(game->map)[(x * MAP_BORDER) + i] == 'D')
						{
							*(game->map)[(x * MAP_BORDER) + i] = 'X';
							x = (bombs[i].power * -1) - 1;
						}
						else if (*(game->map)[(x * MAP_BORDER) + i] == 'I' || *(game->map)[(x * MAP_BORDER) + i] == 'B')
						{
							x = (bombs[i].power * -1) - 1;
						}
						else
						{
							*(game->map)[(x * MAP_BORDER) + i] = 'X';
						}					
					}
				}

				for(y = 0; y <= bombs[i].power; y++)
				{
					if((y) + i > 0)
					{
						if (*(game->map)[(y) + i] == 'D')
						{
							*(game->map)[(y) + i] = 'X';
							y = bombs[i].power + 1;
						}
						else if (*(game->map)[(y) + i] == 'I' || *(game->map)[(y) + i] == 'B')
						{
							y = bombs[i].power + 1;
						}
						else
						{
							*(game->map)[(y) + i] = 'X';
						}
					}
				}

				for(y = 0; y >= (bombs[i].power) * -1; y--)
				{
					if((y) + i > 0)
					{
						if (*(game->map)[(y) + i] == 'D')
						{
							*(game->map)[(y) + i] = 'X';
							y = (bombs[i].power * -1) - 1;
						}
						else if (*(game->map)[(y) + i] == 'I' || *(game->map)[(y) + i] == 'B')
						{
							y = (bombs[i].power * -1) - 1;
						}
						else
						{
							*(game->map)[(y) + i] = 'X';
						}		
					}
				}
				bombs_destroyed[i].index = i;
				bombs_destroyed[i].start_time = 4;
				bombs_destroyed[i].power = 2;
			}
		}
	}
}

void decrease_bomb_destroyer(t_game* game, t_bomb_destroyed *bombs_destroyed)
{
	int i;
	int x;
	int y;
	
	for( i = 0; i < MAP_SIZE; i++)
	{
		if(bombs_destroyed[i].start_time > 0)
		{
			bombs_destroyed[i].start_time--;
			if (bombs_destroyed[i].start_time <= 0)
			{
				for(x = 0; x <= bombs_destroyed[i].power; x++)
				{
					if((x * MAP_BORDER) + i > 0)
					{
						if (*(game->map)[(x * MAP_BORDER) + i] == 'X')
						{
							*(game->map)[(x * MAP_BORDER) + i] = 'E';
						}
					}
				}

				for(x = 0; x >= (bombs_destroyed[i].power) * -1; x--)
				{
					if((x * MAP_BORDER) + i > 0)
					{
						if (*(game->map)[(x * MAP_BORDER) + i] == 'X')
						{
							*(game->map)[(x * MAP_BORDER) + i] = 'E';
						}
					}
				}

				for(y = 0; y <= bombs_destroyed[i].power; y++)
				{
					if((y) + i > 0)
					{
						if (*(game->map)[(y) + i] == 'X')
						{
							*(game->map)[(y) + i] = 'E';
						}			
					}
				}

				for(y = 0; y >= (bombs_destroyed[i].power) * -1; y--)
				{
					if((y) + i > 0)
					{
						if (*(game->map)[(y) + i] == 'X')
						{
							*(game->map)[(y) + i] = 'E';
						}					
					}
				}
			}
		}
	}
}