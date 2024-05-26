NAME = server

SRCS = 	server.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werro


all : $(NAME)

$(NAME) : $(OBJS)
	cc $^ -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ -I server.h

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re