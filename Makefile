
NAME =	server

CC =	clang++ -std:c++98 -Wall -Wextra -Werror

MODELS =	models/ClientRequest.cpp \
			models/Response.cpp \
			models/ServerMessage.cpp \
			models/UID.cpp \

SERVICES =	services/Commands.cpp \
			services/Parser.cpp \
			services/utils.cpp \

SERVER =	Server/ClientService.cpp \
			Server/Server.cpp \
			Server/ServerClientService.cpp \
			Server/ServerModels/Client.cpp \
			Server/Settings/ServerSettings.cpp \
			Server/Sockets/ServerSocket.cpp \
			Server/Sockets/SocketBase.cpp \


SRC =		$(MODELS) $(SERVICES) $(SERVER)

OBJ =		$(SRC:%.cpp=%.o)


all : $(SRC)

clean :

flcean :

re :