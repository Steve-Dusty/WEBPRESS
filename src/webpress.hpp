// SOCKET REQUIREMENTS
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
////

#include <stdlib.h>
#include <thread>

#include "util.hpp"
#include "proto.hpp"
#include "press.hpp"


/*
* This file is more of a server file
* for the server structure. The server
* is really the bulk of the library
* handle so this is fine.
*/
#define MAX_SOCKETS 2048
#define NOT_SOCKET -1

using namespace std;

class pServer {
	private:
		unsigned int sock;
		struct sockaddr_in sockSettings;
		int connectedSockets[MAX_SOCKETS] = {NOT_SOCKET};


		thread acceptThread;
		thread messageThread;
		bool aThreadStop = false;
		bool mThreadStop = false;

		// SOCKOPT
		int reuseAddr = 1;

		// FUNCTIONS //
		void panic(const char errMsg[]) {
			printf("[PSERVER ERR] %s\n", errMsg);
			exit(-1);
		}
		// FUNCTIONS //

	public:
		pServer(unsigned int port) {
			// set socket settings so we can bind later
			sockSettings.sin_family = AF_INET;
			sockSettings.sin_port = htons(port);
			sockSettings.sin_addr.s_addr = INADDR_ANY;


			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
				panic("Could not create socket");
			}

			if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) < 0) {
				panic("Could not set socket option");
			}

			if (bind(sock, (struct sockaddr*)&sockSettings, sizeof(sockSettings)) < 0) {
				panic("Could not bind socket");
			}
		}
		
		void start() {
			this->acceptThread = thread([this]() -> void {
				socklen_t sockSettingsLen = sizeof(this->sockSettings);

				while (1) {
					listen(this->sock, 1);
					int client = accept(
						this->sock,
						(struct sockaddr*)&this->sockSettings,
						&sockSettingsLen
					);

					// ADD LINKED LIST HERE:
				}
			});
			this->messageThread = thread([this]() -> void {
			});

			this->acceptThread.join();
			this->messageThread.join();
		}

		void stop() {
			this->aThreadStop = true;
			this->mThreadStop = true;
		}
};
