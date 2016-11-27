#include <iostream>
#include <set>
#include <streambuf>
#include <string>

#include "ws_server.hpp"

int main(int argc, char* argv[]) {
	ws_server s;

	std::string docroot;
	uint16_t port = 9002;

	if (argc == 1) {
		std::cout << "Usage: " << argv[0] << " [documentroot] [port]" << std::endl;
		return 2;
	}

	if (argc >= 2) {
		docroot = std::string(argv[1]);
	}

	if (argc >= 3) {
		int i = atoi(argv[2]);
		if (i <= 0 || i > 65535) {
			std::cout << "invalid port" << std::endl;
			return 4;
		}

		port = uint16_t(i);
	}

	s.run(docroot, port);
	s.join_all();
	return 0;
}