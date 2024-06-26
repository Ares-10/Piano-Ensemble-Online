NAME = project

SUBDIRS =	lib			\
			client_dir	\
			server_dir	\


all : $(NAME)

$(NAME) :
	for DIRS in $(SUBDIRS); do	\
		$(MAKE) -C $$DIRS;		\
	done
	cp client_dir/client .
	cp server_dir/server .

clean :
	for DIRS in $(SUBDIRS); do		\
		$(MAKE) -C $$DIRS fclean;	\
	done

fclean : clean
	rm -rf server client

re : fclean all

.PHONY : all clean fclean re