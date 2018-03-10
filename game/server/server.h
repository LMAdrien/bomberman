#ifndef SERVER_H
#define SERVER_H

#include "../shared/shared.h"
#include "actions.h"


#define TRUE 1 
#define FALSE 0 
#define PORT 1500

void send_map(t_socket_params socket_params, t_game *game);
void initBuffer(t_game *buffer); 
t_socket_params init_socket(t_socket_params socket_params);
t_socket_params read_client_socket(t_socket_params socket_params, t_game *game, t_bomb *bombs);
t_socket_params clear_socket(t_socket_params socket_params);
t_socket_params check_connexion(t_socket_params socket_params, t_game *game);
t_socket_params set_socket_options(t_socket_params socket_params);
void start_server();
void my_strcat(t_game *game);
void decrease_bomb(t_game* game, t_bomb* bombs, t_bomb_destroyed *bombs_destroyed);
void decrease_bomb_destroyer(t_game* game, t_bomb_destroyed *bombs_destroyed);
int check_if_win_server(t_game *game);

#endif