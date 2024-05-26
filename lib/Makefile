NAME = lib

SUBDIRS = src/minilibx

CFLAGS = -c -Wall -Wextra -Werror


all : $(NAME)

$(NAME) :
	for DIRS in $(SUBDIRS); do	\
		$(MAKE) -C $$DIRS;		\
		cp $$DIRS/*.a . ;		\
	done

clean :
	for DIRS in $(SUBDIRS); do		\
		$(MAKE) -C $$DIRS clean;	\
	done

fclean : clean
	rm -rf *.a

re : fclean all

.PHONY : all clean fclean re