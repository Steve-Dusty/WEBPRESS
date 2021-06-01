// SOCKET REQUIREMENTS
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
////

#include <unistd.h>
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
#define NOT_SOCKET -1
#define MAX_PACKET_LENGTH 65535 // default TCP max packet length

using namespace std;

// SOCKET LINKED LIST //
typedef struct _inRef {
    int sock;

    // inRef means in inner reference
    struct _inRef *forward;
    struct _inRef *backward;
} sockLink;

sockLink *appendSockLink(sockLink *node) {
    sockLink *newNode;
    newNode = (sockLink*)malloc(sizeof(sockLink));

    // append
    node->forward = newNode;
    newNode->backward = node;

    newNode->sock = NOT_SOCKET;
    return newNode;
}

void removeSockLink(sockLink *node) {
    // FREE AFTER EVERYTHING

    // first make the node behind us use the node that is ahead of us
    node->backward->forward = node->forward;

    // now make the node ahead of us point to the node before us
    node->forward->backward = node->backward;

    // clean up and delete the node given to us
    free(node);
}

// its just to make the syntax feel a little more elegant
sockLink *nextSockLink(sockLink *node) {
    return node->forward;
}
// SOCKET LINKED LIST //



class pServer {
    private:
        unsigned int sock;
        struct sockaddr_in sockSettings;
        sockLink head = {
            NOT_SOCKET,
            NULL,
            NULL
        };


        thread acceptThread;
        thread messageThread;
        bool first = true;
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
                        if (this->aThreadStop) {
                            std::terminate();
                        }

                        listen(this->sock, 1);
                        int client = accept(
                            this->sock,
                            (struct sockaddr*)&this->sockSettings,
                            &sockSettingsLen
                        );

                        sockLink *node = &this->head;
                        while (node->sock != NOT_SOCKET) { // go down linked list
                            node = nextSockLink(node);
                        }

                        if (this->first) {
                            this->head.sock = client;
                            this->first = false;
                        } else {
                            sockLink *newClient = appendSockLink(node);
                            newClient->sock = client;
                        }
                        printf("Recived connection\n");
                    }
            });


            this->messageThread = thread([this]() -> void {
                socklen_t sockSettingsLen = sizeof(this->sockSettings);

                while (1) {
                    if (this->mThreadStop) {
                        std::terminate();
                    }

                    // loop through all clients
                    sockLink *node = &this->head;
                    while (node->sock != NOT_SOCKET) {
                        char packet[MAX_PACKET_LENGTH];
                        read(
                            node->sock,
                            packet,
                            sizeof(packet)
                        );

                        // TODO process packets for now i'll just print them out
                        printf("RECV PACKET %s\n", packet);

                        if (node->forward == NULL) {
                            break;
                        } else {
                            node = nextSockLink(node);
                        }
                    }
                }
            });

            this->acceptThread.join();
            this->messageThread.join();
        }

        void stop() {
            this->aThreadStop = true;
            this->mThreadStop = true;
        }
};
