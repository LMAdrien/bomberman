#ifndef __JOIN_MENU_H__
#define __JOIN_MENU_H__

int load_menu_join();
void draw_menu_join();
int init_join_menu();
void free_join_menu();
void handle_key_press(SDL_Keysym key);
void append_to_input(char c);
void remove_from_input();
int get_nb_client_connected(char *server_map);

#endif
