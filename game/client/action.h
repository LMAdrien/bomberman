#ifndef __ACTION_H__
#define __ACTION_H__

int create_game();
int join_game();
void *run_server_thread(void *arg);
int connect_to_server(char *ip, int port);
int send_action_to_server(char *action, int sock);

#endif