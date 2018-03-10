#ifndef SHARED_H
#define SHARED_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_PLAYERS 4
#define MAP_SIZE 225 /* 15x15 */
#define MAP_BORDER 15

typedef char t_map[MAP_SIZE];

typedef struct  s_client_request
{
    unsigned int  magic;                  /* Un magic number commun entre le client et le serveur, ou l'identifiant d'un type de structure */
    int           x_pos;                  /* La position x souhaitée par le client */
    int           y_pos;                  /* La position y souhaitée par le client */
    int           dir;                    /* La direction souhaitée par le client */
    char          *action;                /* Une action que le client souhaite executer */
    int           speed;                  /* La vitesse du joueur */
    int           ckecksum;               /* Un checksum simple */
}               t_client_request;

typedef struct s_player_infos
{
    int   connected;
    int   alive;
    int   x_pos;
    int   y_pos;
    int   current_dir;
    int   current_speed;
    int   max_speed;
    int   bombs_left;
    int   bombs_capacity;
    int   frags;
}               t_player_infos;

typedef struct          s_game
{
    t_player_infos        player_infos[MAX_PLAYERS];
    t_map                 *map;
    char                  *map_string;
}                       t_game;

typedef struct          s_socket_params
{
    int parent_socket; 
    int addrlen;
    int child_socket; 
    int client_socket[MAX_PLAYERS]; 
    int activity;
    int sd;
	int max_sd;
    char buffer[1024];
    fd_set readfds;
	struct sockaddr_in address;
    struct timeval tv;
}                       t_socket_params;     

typedef struct          s_bomb
{
    int start_time;
    int index;
    int power;
}                       t_bomb;

typedef struct          s_bomb_destroyed
{
    int start_time;
    int index;
    int power;
}                       t_bomb_destroyed;        

#endif