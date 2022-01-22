#include "Server/Server.hpp"
#include "Server/Settings/ServerSettings.hpp"

int main(int ac, char **av) {
	ServerSettings *settings;

	if (ac == 3) {
		try {
			settings = new ServerSettings(av[1], av[2]);
		} catch (ServerSettingsException &e) {
			std::cout << e.what() << std::endl;
		}
	} else {
		printf("Error args.\n./ircserv <port> <password>\n");
		exit(-1);
	}

	try {
		Server server = Server(settings);
		server.run();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}