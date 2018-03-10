#include "common.h"
#include "main_menu.h"
#include "join_menu.h"
#include "action.h"

int init_main_menu()
{
    g_menu.choice_selection = 0;
    g_menu.choice_label[0] = "Main Menu";  
    g_menu.choice_label[1] = "Create Game";
    g_menu.choice_label[2] = "Join";
    g_menu.choice_label[3] = "Exit";
    g_menu.font = TTF_OpenFont(BASE_ASSETS_PATH "/ttf/bm.ttf", 65);
    if(g_menu.font == NULL)
    {
        return (-1);
    }
    return (0);
}

void draw_main_menu()
{
    int i;
    int offset = 10;
    
    for(i = 0; i <= 3; i++)
    {
        if(i == g_menu.choice_selection + 1)
        {
            g_menu.text_color.r = 178;
            g_menu.text_color.g = 255;
            g_menu.text_color.b = 102;
        }
        else
        {
            g_menu.text_color.r = 255;
            g_menu.text_color.g = 255;
            g_menu.text_color.b = 255;
        }

        g_menu.surfaces[i] = TTF_RenderText_Solid(g_menu.font, g_menu.choice_label[i], g_menu.text_color);
        g_menu.texture = SDL_CreateTextureFromSurface(g_game->renderer,g_menu.surfaces[i]);
        g_menu.textWidth[i] =  0;
        g_menu.textHeight[i] = 0;

        SDL_QueryTexture( g_menu.texture, NULL, NULL, &g_menu.textWidth[i], & g_menu.textHeight[i]);
        SDL_Rect dstrect = { 100, offset = offset + OFFSET_POLICE , g_menu.textWidth[i], g_menu.textHeight[i] };
        SDL_RenderCopy(g_game->renderer ,  g_menu.texture, NULL, &dstrect);
        SDL_RenderPresent(g_game->renderer );
    }
}

void navigate(SDL_Event event){
    switch (event.key.keysym.sym)
    {
        case SDLK_UP:
        if(g_menu.choice_selection < 1)
        {
            g_menu.choice_selection = 2;
        }    
        else
        { 
            g_menu.choice_selection--; 
        }
        break;
        
        case SDLK_DOWN:
        if(g_menu.choice_selection > 1)
        {
            g_menu.choice_selection = 0;
        }    
        else
        { 
            g_menu.choice_selection++; 
        }
        break; 
        
        case SDLK_RETURN:
        navigate_main_menu();
        break;
    }           
}

void free_main_menu()
{
    int i;
    SDL_DestroyTexture( g_menu.texture );
    for(i = 0; i <= 3; i++)
    {
       SDL_FreeSurface( g_menu.surfaces[i]);
    }
}

int navigate_main_menu()
{
    if(g_menu.choice_selection == 0)
    {
        /* Create Game*/
        if (create_game() != 0){
            /*Handle Error*/
        }
    }
    else if(g_menu.choice_selection == 1)
    {
        /* Join Game*/
        if (load_menu_join() == -1)
            g_menu.exit_game = true;
    }
    else if(g_menu.choice_selection == 2)
    {
        /* Exit */
        g_menu.exit_game = true;
    }
    return (0);
}