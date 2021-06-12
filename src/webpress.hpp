// IOSTREAM
#include <iostream>

// SOCKET REQUIREMENTS
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
////

#include <unistd.h>
#include <stdlib.h>

#include "util.hpp"
#include "proto.hpp"
#include "press.hpp"

/*
 * This file is more of a server file
 * for the server structure. The server
 * is really the bulk of the library
 * handle so this is fine.
 */
#define DEFAULT_PORT 8080
#define DEFAULT_QUEUE 1
#define MAX_PACKET_LENGTH 65535 // TCP max packet length

using namespace std;

class pServer {
    private:
        unsigned int sock;
        struct sockaddr_in sockSettings;

        // constructor params //
        unsigned int port = DEFAULT_PORT;
        unsigned int queue = DEFAULT_QUEUE;
        // constructor params //

        // SOCKOPT
        int reuseAddr = 1;

        // FUNCTIONS //
        void panic(const char errMsg[]) {
            cout << "[PSERVER ERR]" << "\n" errMsg;
            exit(-1);
        }
        // FUNCTIONS //

    public:
        pServer(unsigned int _port, unsigned int _queue) {
            // set constructor params
            port = _port;
            queue = _queue;
            ////

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
            socklen_t sockSettingsLen = sizeof(this->sockSettings);

            while (1) {
                listen(this->sock, this->queue);
                int client = accept(
                    this->sock,
                    (struct sockaddr*)&this->sockSettings,
                    &sockSettingsLen
                );
                cout << "Recived connection";

                char packet[MAX_PACKET_LENGTH];
                read(
                    client,
                    packet,
                    sizeof(packet)
                );
                cout << "Recived packet" << "\n" << packet;

                shutdown(client, SHUT_RDWR); // close the client
            }
        }
};
