CC=		g++ -fPIC -shared -g --std=c++11 -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/bsoncxx/v_noabi -L/usr/local/lib -lmongocxx -lbsoncxx

NAME=		../splugin_mongo.so

RM=		rm -rf

SRC=		Mongo.cpp

all:		$(NAME)

$(NAME):
		$(CC) $(SRC) -o $(NAME)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
