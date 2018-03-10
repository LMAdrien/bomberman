#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define APP_TITLE "Bomberman V2.0"
#define BASE_ASSETS_PATH "./ressources"
#define OFFSET_POLICE 100
#define SIZE_X 600
#define SIZE_Y 600

enum MENU_LEVEL 
{
  MAIN,
  JOIN,
  CREATE,
};


typedef struct s_clinet_game 
{
  int socket;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Event event;
  Uint32 timer;
  int running;
  SDL_Texture* textures[14];
  int (*action_list[6])(int);
  pthread_t server_thread;
}                 t_client_game;


typedef struct s_menu 
{
  SDL_Color text_color;
  TTF_Font* font;
  char* choice_label[4];
  SDL_Texture *texture;
  SDL_Surface *surfaces[4];
  int textWidth[4];
  int textHeight[4];
  int choice_selection;
  enum MENU_LEVEL menu;
  bool exit_game;
}                 t_menu;

typedef struct s_join_menu 
{
  SDL_Color text_color;
  TTF_Font* font;
  char* info;
  char* ip_input;
  SDL_Texture *info_texture;
  SDL_Texture *ip_texture;
  SDL_Surface *info_surface;
  SDL_Surface *ip_surface;
  int info_text_width;
  int info_text_height;
  int ip_text_width;
  int ip_text_height;
  bool exit_menu;
}               t_join_menu;

typedef struct s_game_ressources
{
  SDL_Texture* wall_img;
  SDL_Texture* block_img;
  SDL_Texture* ground_img;
  SDL_Texture* dynamite_img;
  SDL_Texture* explosion_img;
  SDL_Texture* player1_img;
  SDL_Texture* player2_img;
  SDL_Texture* player3_img;
  SDL_Texture* player4_img;
}              t_game_ressources;


/*----(prototype)-------------------------------------- */


int init_sdl();
void exit_game();


/*----(globals)-------------------------------------- */
t_client_game* g_game;
t_menu g_menu;
t_join_menu g_join_menu;
int server_is_lunched;
int can_start_game;
int game_sarted;

#endif