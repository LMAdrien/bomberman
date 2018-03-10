#include "game.h"
#include "action.h"
#include "../shared/shared.h"

void game_loop(int sock)
{
    Uint32 startclock = 0;
    Uint32 deltaclock = 0;
    Uint32 currentFPS = 0;
    char caption[64];
    int sleep;

    char *server_reply;
    int we_have_a_winner;
    SDL_Event event;
    t_game_ressources *ressources;
    can_start_game = 0;
    game_sarted = 0;
    we_have_a_winner = 0;

    if ((server_reply = (char *)malloc(sizeof(char) * MAP_SIZE)) == NULL){
        /* Handle error */
        printf("server reply client malloc failed\n");
    }

    if ((ressources = (t_game_ressources *)malloc(sizeof(t_game_ressources))) == NULL){
        /* Handle error */
        printf("ressources malloc failed\n");
    }

    init_ressources(ressources);
    startclock = SDL_GetTicks();
    
    while(we_have_a_winner == 0)
    {
        event.key.keysym.scancode = 0;
        SDL_RenderClear(g_game->renderer);
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_DOWN:
                        send_action_to_server("D", sock);
                        break;
                    case SDL_SCANCODE_UP:
                        send_action_to_server("U", sock);
                        break;
                    case SDL_SCANCODE_LEFT:
                        send_action_to_server("L", sock);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        send_action_to_server("R", sock);
                        break;
                    case SDL_SCANCODE_SPACE:
                        send_action_to_server("S", sock);
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (can_start_game == 1)
                        {
                            game_sarted = 1;
                            send_action_to_server("A", sock);
                        }
                        break;
                    default:
                        break;       
                }
                break; 
        }
        switch (event.type)
        {
            case SDL_QUIT:
                exit(EXIT_FAILURE);
                break; 
        }

        deltaclock = SDL_GetTicks() - startclock;
        startclock = SDL_GetTicks();

        if ( deltaclock != 0 )
            currentFPS = 1000 / deltaclock;
        
        sprintf( caption, "Frames Par Seconde: %d", currentFPS);

        //Receive data from the server
        if( recv(sock , server_reply , 225, 0) < 225)
        {
            //printf("No data recived from server\n");
        }
        else
        {
            //printf("Map recived draw map\n");
            we_have_a_winner = draw_map(server_reply, ressources, caption);
        }

        sleep = (1.0 / 60 * 1000) - deltaclock;

        if(sleep < 0) { 
            sleep = 0; 
        }
        SDL_Delay(sleep);
        
    }
}

int draw_map(char* map, t_game_ressources *ressources, char* fps)
{
    int i;
    int width_img, height_img;
    int xpos, ypos;
    int player_count;
    int we_have_a_winner;

    SDL_Surface *fps_surface;
    SDL_Texture *fps_texture;
    TTF_Font *message_font;
    message_font = TTF_OpenFont(BASE_ASSETS_PATH "/ttf/bm.ttf", 20);
    SDL_Color message_color;
    message_color.r = 0;
    message_color.g = 0;
    message_color.b = 0;
    int message_width =  0;
    int message_height = 0;


    width_img = SIZE_X / 15;
    height_img = SIZE_Y / 15;
    xpos = SIZE_X / 15;
    ypos = SIZE_Y / 15;
    player_count = 0;

    if (map[0] == 'S')
    {
        game_sarted = 1;
    }

    for(i = 0; i < MAP_SIZE; i++)
    {
        if (map[i] == 'B' || map[i] == 'S')
        {
            SDL_QueryTexture( ressources->wall_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->wall_img, NULL, &dstrect);
        }
        else if (map[i] == 'I')
        {
            SDL_QueryTexture( ressources->wall_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->wall_img, NULL, &dstrect);
        }
        else if (map[i] == 'E')
        {
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);
        }
        else if (map[i] == 'D')
        {
            SDL_QueryTexture( ressources->block_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->block_img, NULL, &dstrect);
        }
        else if (map[i] == 'T')
        {
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->dynamite_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->dynamite_img, NULL, &dstrect2);
        }
        else if (map[i] == 'X')
        {
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->explosion_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->explosion_img, NULL, &dstrect2);
        }
        else if (map[i] == '1')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->player1_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player1_img, NULL, &dstrect2);
        }
        else if (map[i] == '2')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->player2_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player2_img, NULL, &dstrect2);
        }
        else if (map[i] == '3')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->player3_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player3_img, NULL, &dstrect2);
        }
        else if (map[i] == '4')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->player4_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player4_img, NULL, &dstrect2);
        }
        else if (map[i] == '5')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->dynamite_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect1 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->dynamite_img, NULL, &dstrect1);

            SDL_QueryTexture( ressources->player1_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player1_img, NULL, &dstrect2);
        }
        else if (map[i] == '6')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);
            
            SDL_QueryTexture( ressources->dynamite_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect1 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->dynamite_img, NULL, &dstrect1);

            SDL_QueryTexture( ressources->player2_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player2_img, NULL, &dstrect2);
        }
        else if (map[i] == '7')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->dynamite_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect1 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->dynamite_img, NULL, &dstrect1);

            SDL_QueryTexture( ressources->player3_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player3_img, NULL, &dstrect2);
        }
        else if (map[i] == '8')
        {
            player_count++;
            SDL_QueryTexture( ressources->ground_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->ground_img, NULL, &dstrect);

            SDL_QueryTexture( ressources->dynamite_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect1 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->dynamite_img, NULL, &dstrect1);

            SDL_QueryTexture( ressources->player4_img, NULL, NULL, &width_img, &height_img );
            SDL_Rect dstrect2 = { ((i % 15) * xpos), ((( i - (i % 15)) / 15) * ypos) , xpos, ypos };
            SDL_RenderCopy(g_game->renderer ,  ressources->player4_img, NULL, &dstrect2);
        }
    }
    wait_for_players(player_count);
    we_have_a_winner = check_if_win(player_count);

    fps_surface = TTF_RenderText_Solid(message_font, fps, message_color);
    fps_texture = SDL_CreateTextureFromSurface(g_game->renderer, fps_surface);
    SDL_QueryTexture( fps_texture, NULL, NULL, &message_width, &message_height);
    SDL_Rect rect = { 150, 10 , message_width, message_height };
    SDL_RenderCopy(g_game->renderer ,  fps_texture, NULL, &rect);

    SDL_RenderPresent(g_game->renderer);
    return (we_have_a_winner);
}

void wait_for_players(int player_count)
{
    int message_width;
    int message_height;
    SDL_Color message_color;
    SDL_Surface *message_surface;
    SDL_Texture *message_texture;
    TTF_Font *message_font;
    
    message_font = TTF_OpenFont(BASE_ASSETS_PATH "/ttf/bm.ttf", 20);

    if(message_font == NULL)
    {
        //handle error
    }

    if (game_sarted == 0)
    {
        message_color.r = 0;
        message_color.g = 0;
        message_color.b = 0;
        message_width =  0;
        message_height = 0;

        if(player_count < 2)
        {
            message_surface = TTF_RenderText_Solid(message_font, "Not enought players", message_color);
        }   
        else if (server_is_lunched == 1)
        {
            message_surface = TTF_RenderText_Solid(message_font, "Press ENTER to start the game", message_color);
            can_start_game = 1;
        }
        else
        {
            message_surface = TTF_RenderText_Solid(message_font, "Wait start", message_color);
        }

        message_texture = SDL_CreateTextureFromSurface(g_game->renderer, message_surface);
        SDL_QueryTexture( message_texture, NULL, NULL, &message_width, &message_height);
        SDL_Rect rect = { 10, 250 , message_width, message_height };
        SDL_RenderCopy(g_game->renderer ,  message_texture, NULL, &rect);
    }
}

int check_if_win(int player_count)
{
    if (game_sarted == 1 && player_count <= 1)
    {
        return (1);
    }
    return (0);
}

void init_ressources(t_game_ressources *ressources)
{
    SDL_Surface* surface;
    surface= IMG_Load(BASE_ASSETS_PATH "/images/wall.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->wall_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->wall_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/block.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->block_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->block_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/ground.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->ground_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->ground_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/dynamite.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->dynamite_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->dynamite_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/explosion.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->explosion_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->explosion_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/player1.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->player1_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->player1_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/player2.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->player2_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->player2_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/player3.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->player3_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->player3_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }

    surface = IMG_Load(BASE_ASSETS_PATH "/images/player4.png");
    if(!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
    }
    ressources->player4_img = SDL_CreateTextureFromSurface(g_game->renderer, surface);
    if (ressources->player4_img == NULL) 
    {
        printf("CreateRGBSurface failed: %s\n", SDL_GetError());
    }
}