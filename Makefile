NAME = project

SERVER_SRCS = server.c
CLIENT_SRCS = client.c

SERVER_OBJS = server.o
CLIENT_OBJS = client.o

SUBDIRS = minilibx

all : $(NAME)

$(NAME) : server client

server : $(SERVER_OBJS)
	cc server.o -o server

client : $(CLIENT_OBJS)
	for DIRS in $(SUBDIRS); do	\
		$(MAKE) -C $$DIRS;		\
	done
	cp minilibx/libmlx.a .
	cc -L. -lmlx -framework OpenGL -framework AppKit $^ -o client

%.o: %.c
	cc -Imlx -c $< -o $@

clean :
	for DIRS in $(SUBDIRS); do		\
		$(MAKE) -C $$DIRS clean;	\
	done
	rm -rf libmlx.a
	rm -rf server.o client.o

fclean : clean
	rm -rf server client

re : fclean all

.PHONY : all clean fclean re