/**
 * @file        client.h
 * @author      Lad Dhawal Umesh
 * @developedBy Lad Dhawal Umesh
 * @brief       Declarations for client.
 * @copyright   (c) 2026 Lad Dhawal Umesh. All rights reserved.
 */

#ifndef CLIENT_H
#define CLIENT_H

// Headers
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

// MACROS
#define SERVER_IP "192.168.134.174"
#define CLIENT_PORT 8080

#define BUFFER_SIZE 1024

#define SUCCESS 0
#define FAILURE -1

#define PROTOCOL_FAMILY AF_INET
#define SOCKET_TYPE SOCK_STREAM
#define PROTOCOL 0

#define INIT_0 0
#define INIT_1 1
#define INIT_M_1 -1

// Global Variables
struct sockaddr_in sAddress;

// Function Declarations
int iInitSocketClient(int *ipSocketfd);
int iCloseSocketClient(int *ipSocketfd);
int iConnectToServer(int *ipSocketfd);

#endif // CLIENT_H