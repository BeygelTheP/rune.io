#include <iostream>
#include <string>

#include "server/ws_server.hpp"
#include "world/world.hpp"

int main(int argc, char* argv[]) {
	runeio::ws_server server;
	runeio::world world;
	uint16_t port = 9002;

	if (argc == 1 || argc > 2) {
		std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
		return 2;
	}

	if (argc == 2) {
		int i = atoi(argv[1]);
		if (i <= 0 || i > 65535) {
			std::cout << "invalid port" << std::endl;
			return 4;
		}

		port = uint16_t(i);
	}

	server.set_listen_channel(world.get_channel());
	world.set_server_channel(server.get_channel());

	server.run(port);
	world.run();

	// Waiting for all threads done.
	server.join_all();
	world.join_all();

	return 0;
}