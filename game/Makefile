SERVER = server
CLIENT = client
GAME   = bomberman
BUILDS = builds

SERVER_SRCS = $(wildcard server/*.c)
CLIENT_SRCS = $(wildcard client/*.c)

SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

CC = gcc
RM = rm -f
CFLAGS = -lpthread -std=gnu99 -lSDL2 -lSDL2_image -lSDL2_ttf -pedantic -W -Wall -Werror

all : allinone

dir:
	mkdir -p $(BUILDS)

server : $(SERVER_OBJS) dir
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(BUILDS)/$(SERVER)

client : $(CLIENT_OBJS) dir
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $(BUILDS)/$(CLIENT)

allinone: $(SERVER_OBJS) $(CLIENT_OBJS) dir
	$(CC) $(CFLAGS) $(SERVER_OBJS) $(CLIENT_OBJS) -o $(BUILDS)/$(GAME)

clean :
	$(RM) $(SERVER_OBJS)
	$(RM) $(CLIENT_OBJS)

fclean : clean
	$(RM) $(BUILDS)/*

re : fclean all