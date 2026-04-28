/**
 * @file        server.h
 * @author      Lad Dhawal Umesh
 * @developedBy Lad Dhawal Umesh
 * @brief       Declarations for server.
 * @copyright   (c) 2026 Lad Dhawal Umesh. All rights reserved.
 */

#ifndef SERVER_H
#define SERVER_H

// Headers
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>

// MACROS
#define SERVER_PORT 8080
#define SERVER_IP "192.168.134.174"

#define BUFFER_SIZE 1024

#define SUCCESS 0
#define FAILURE -1

#define PROTOCOL_FAMILY AF_INET
#define SOCKET_TYPE SOCK_STREAM
#define PROTOCOL 0

#define INIT_0 0
#define INIT_1 1
#define INIT_M_1 -1

#define BACKLOG 3

#define MAX_CLIENTS 30

// Global Variables
struct sockaddr_in sAddress;
int iClientSockets[MAX_CLIENTS];
fd_set readfds;

// Function Declarations
int iInitSocketServer(int *ipSocketfd);
int iCloseSocketServer(int *ipSocketfd);
int iAcceptClientConnection(int *ipSocketfd);

#endif // SERVER_H