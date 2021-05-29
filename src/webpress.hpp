// SOCKET REQUIREMENTS
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
////


#include "util.hpp"
#include "proto.hpp"
#include "press.hpp"


/*
 * This file is more of a server file
 * for the server structure. The server
 * is really the bulk of the library
 * handle so this is fine.
 */

class pServer {
	private:
		unsigned int socket;
		struct sockaddr_in sockSettings;

	public:
		pServer() {
		}
};
