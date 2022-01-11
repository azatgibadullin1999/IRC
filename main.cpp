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
	} else if (ac == 4) {
		try {
			settings = new ServerSettings(av[1], av[2], av[3]);
		} catch (ServerSettingsException &e) {
			std::cout << e.what() << std::endl;
		}
	} else {
		printf("Error");
		exit(-1);
	}

	Server server = Server(settings);
	server.run();
}