#include "common.h"
#include "action.h"
#include "game.h"
#include "../server/server.h"
#include <fcntl.h>
#include <unistd.h>

int create_game(){
    int server_thread;
    if((server_thread = pthread_create(&g_game->server_thread, NULL, run_server_thread, NULL)) == -1) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    while(server_is_lunched == 0){
        printf("Creating server...\n");
        sleep(1);
    }

    connect_to_server("127.0.0.1", 1500);
    pthread_cancel(server_thread);
    return (0);
}

int join_game()
{
    return (0);
}

void *run_server_thread(void *arg){
    start_server();
    /* Pour enlever le warning */
    (void) arg;
    printf("End thread.\n");
    pthread_exit(NULL);
}

int connect_to_server(char *ip, int port){
    int sock;
    int flags;
    struct sockaddr_in server;
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    /*Connect to server*/
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("Connected\n");

    if ((flags = fcntl(sock, F_GETFL, 0)) < 0) 
    { 
        /* Handle error */
        printf("Error flags\n"); 
    } 

    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) 
    { 
        /* Handle error */ 
        printf("Error fcntl\n");
    } 

    game_loop(sock);
     
    close(sock);
    return (0);
}

int send_action_to_server(char *action, int sock)
{
    if (game_sarted == 1)
    {
        //Send data to server
        if( (size_t)send(sock , action , 1 , 0) != 1)
        {
            printf("Send failed \n");
            return (-1);
        }
    }
    
    return (0);
}