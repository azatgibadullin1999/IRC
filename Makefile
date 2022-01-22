NAME =	ircserv

CC =	clang++ -std=c++98

MODELS =	models/ClientRequest.cpp \
			models/Response.cpp \
			models/UID.cpp \

SERVICES =	services/Commands.cpp \
			services/Parser.cpp \
			services/utils.cpp \

SERVER =	Server/ClientService.cpp \
			Server/Sockets/SocketBase.cpp \
			Server/Sockets/ServerSocket.cpp \
			Server/Server.cpp \
			Server/ConnectionsService.cpp \
			Server/ServerModels/Client.cpp \
			Server/Settings/ServerSettings.cpp \

INC =		-I./models/ \
			-I./services/ \
			-I./Server/ \
			-I./Server/Settings \
			-I./Server/ServerModels \
			-I./Server/Sockets \


SRC =		$(MODELS) $(SERVICES) $(SERVER) main.cpp

OFILE = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OFILE)
	$(CC) $(INC) $(OFILE) -o $(NAME)

clean: 
	rm -f $(OFILE)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re %.o