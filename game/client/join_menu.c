#include "common.h"
#include "join_menu.h"
#include "action.h"



int init_join_menu(){
    g_join_menu.info = "Enter the ip address of the server :";
    g_join_menu.ip_input = '\0';
    g_join_menu.font = TTF_OpenFont(BASE_ASSETS_PATH "/ttf/bm.ttf", 30);
    if(g_join_menu.font == NULL)
    {
        return (-1);
    }
    return (0);
}

int load_menu_join()
{
    int return_code = 0;
    SDL_Event event;
    g_join_menu.exit_menu = false;

    if (init_join_menu() == -1)
    {
        return -1;
    }

    while (!g_join_menu.exit_menu)
    {
        SDL_RenderClear(g_game->renderer);
        draw_menu_join();
        SDL_WaitEvent(&event);
        SDL_Delay(20);

        switch (event.type)
        {
            case SDL_QUIT:
                g_join_menu.exit_menu = true;
                return_code = -1;
                break;
            case SDL_KEYDOWN:
                handle_key_press(event.key.keysym);
                break;
        }
    }
    free_join_menu();
    return (return_code);
}

void draw_menu_join(){
    int offset = 10;

    g_join_menu.text_color.r = 255;
    g_join_menu.text_color.g = 255;
    g_join_menu.text_color.b = 255;

    g_join_menu.info_surface = TTF_RenderText_Solid(g_join_menu.font, g_join_menu.info, g_join_menu.text_color);
    g_join_menu.ip_surface = TTF_RenderText_Solid(g_join_menu.font, g_join_menu.ip_input, g_join_menu.text_color);
    g_join_menu.info_texture = SDL_CreateTextureFromSurface(g_game->renderer,g_join_menu.info_surface);
    g_join_menu.ip_texture = SDL_CreateTextureFromSurface(g_game->renderer,g_join_menu.ip_surface);

    g_join_menu.info_text_width =  0;
    g_join_menu.info_text_height = 0;
    g_join_menu.ip_text_width =  0;
    g_join_menu.ip_text_height = 0;

    SDL_QueryTexture( g_join_menu.info_texture, NULL, NULL, &g_join_menu.info_text_width, & g_join_menu.info_text_height );
    SDL_Rect dstrect = { 100, offset = offset + OFFSET_POLICE , g_join_menu.info_text_width, g_join_menu.info_text_height };
    SDL_RenderCopy(g_game->renderer ,  g_join_menu.info_texture, NULL, &dstrect);


    SDL_QueryTexture( g_join_menu.ip_texture, NULL, NULL, &g_join_menu.ip_text_width, & g_join_menu.ip_text_height );
    SDL_Rect ip_strect = { 100, offset = offset + OFFSET_POLICE , g_join_menu.ip_text_width, g_join_menu.ip_text_height };
    SDL_RenderCopy(g_game->renderer ,  g_join_menu.ip_texture, NULL, &ip_strect);
    SDL_RenderPresent(g_game->renderer );
}

void handle_key_press(SDL_Keysym key){
    const char * str = NULL;

    if (key.scancode >= 89 && key.scancode <= 98)/* Keypad numeric */
    {
        str = SDL_GetKeyName( key.sym );
        append_to_input(str[strlen(str) - 1]);
    }
    else if (key.scancode == 40 || key.scancode == 88)/* Enter key or keypad enter key */
    {
        /* Join server to play */
        printf("%s Join server \n", g_join_menu.ip_input);
        connect_to_server((char *)g_join_menu.ip_input, 1500);
    }
    else if (key.scancode == 41 )/* Escape key */
    {
        g_join_menu.exit_menu = true;
    }
    else if (key.scancode == 42)/* Backspace key */
    {
        remove_from_input();
    }
    else if (key.scancode == 55)/* : key */
    {
        append_to_input(':');
    }
    else if (key.scancode == 99)/* keypad period key */
    {
        append_to_input('.');
    }
}

void  append_to_input(char c) {
    int len;

    if (g_join_menu.ip_input != NULL)
    {
        len = strlen(g_join_menu.ip_input);
        g_join_menu.ip_input[len] =  c;
        g_join_menu.ip_input[len + 1] = '\0';
    }
    else
    {
        g_join_menu.ip_input = malloc(sizeof(char) * 30);
        g_join_menu.ip_input[0] = c;
        g_join_menu.ip_input[1] = '\0';
    }
}

void  remove_from_input() {
    int len;

    if (g_join_menu.ip_input != NULL)
    {
        len = strlen(g_join_menu.ip_input);
        g_join_menu.ip_input[len - 1] =  '\0';
    }
}

void free_join_menu()
{
    SDL_DestroyTexture( g_join_menu.info_texture );
    SDL_DestroyTexture( g_join_menu.ip_texture );
    SDL_FreeSurface( g_join_menu.info_surface);
    SDL_FreeSurface( g_join_menu.ip_surface);
}

int get_nb_client_connected(char *server_map)
{
  int i;
  int count = 0;
  for (i = 0; i < 225; i++)
  {
    if(server_map[i] == '1' || server_map[i] == '2' || server_map[i] == '3' || server_map[i] == '4')
    {
      count++;
    }
  }
  return (count);
}
