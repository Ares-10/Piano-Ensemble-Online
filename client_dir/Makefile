NAME = client

SRCS = 	client.c 		\
		utils.c			\

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror


all : $(NAME)

$(NAME) : $(OBJS)
	cc -L. -framework OpenGL -framework AppKit $^ ../lib/libmlx.a -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ -I client.h

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re