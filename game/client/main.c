#include "common.h"
#include "main_menu.h"
#include "join_menu.h"
#include "action.h"
#include <pthread.h>

int main()
{
    server_is_lunched = 0;
    SDL_Event event;    

    if( init_sdl() == -1) 
        return EXIT_FAILURE;

    if(init_main_menu() == -1)
        return EXIT_FAILURE;
    g_menu.exit_game = false;
    
    while (!g_menu.exit_game)
    {
        SDL_RenderClear(g_game->renderer);
        draw_main_menu();
        SDL_WaitEvent(&event);
        SDL_Delay(20); 
        switch (event.type)
        {
            case SDL_QUIT:
                g_menu.exit_game = true;
                break;
            case SDL_KEYDOWN:
               navigate(event);
               break;       
        } 
    }
    free_main_menu();
    exit_game();
    return (0);
}

int init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
	    return (-1);
    }
    g_game =  malloc(sizeof(t_client_game));
    if (g_game == NULL)
    {
        return (-1);
    }
    g_game->window = SDL_CreateWindow(APP_TITLE, 100, 100, SIZE_X, SIZE_Y, SDL_WINDOW_SHOWN);  
    if (g_game->window  == NULL)
    {
	    return (-1);
    }
    if(TTF_Init() == -1)   
    {
        return (-1);
    }
    g_game->renderer = SDL_CreateRenderer(g_game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_game->renderer == NULL)
    {
       return (-1);
    }
    g_game->timer = SDL_GetTicks();
    return (0);
}

void exit_game(){
    SDL_DestroyRenderer(g_game->renderer);
    SDL_DestroyWindow(g_game->window);
    SDL_Quit();
}