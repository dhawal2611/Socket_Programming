/**
 * @file        server.c
 * @brief       Socket server code
 * @details     Create a socket server and wait for a 
 *              client to connect to it.
 * @author      Lad Dhawal Umesh
 * @developedBy Lad Dhawal Umesh
 * @date        2026-04-14
 * @version     1.0.0
 * @copyright   (c) 2026 Lad Dhawal Umesh. All rights reserved.
 */

// Include Header
#include "server.h"

/**
 * @brief Initialize the socket server.
 * @param ipSocketfd 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will initialize the socket server.
 */
int iInitSocketServer(int *ipSocketfd) {
    // Implementation for initializing the socket server
    int iOpt = INIT_0;

    // Create socket file descriptor
    *ipSocketfd = socket(PROTOCOL_FAMILY, SOCKET_TYPE, PROTOCOL);
    if (*ipSocketfd < INIT_0) {
        perror("socket failed");
        return FAILURE;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(*ipSocketfd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &iOpt,
                   sizeof(iOpt))) {
        perror("setsockopt");
        return FAILURE;
    }

    sAddress.sin_family = PROTOCOL_FAMILY;
    sAddress.sin_port = htons(SERVER_PORT);
    
    // sAddress.sin_addr.s_addr = INADDR_ANY;
    // OR
    // Convert string IP to binary format and assign to sin_addr
    if (inet_pton(PROTOCOL_FAMILY, SERVER_IP, &sAddress.sin_addr) <= INIT_0) {
        perror("Invalid address/Address not supported");
        return FAILURE;
    }

    if (bind(*ipSocketfd, (struct sockaddr*)&sAddress,
             sizeof(sAddress)) < INIT_0) {
        perror("bind failed");
        return FAILURE;
    }   

    if (listen(*ipSocketfd, BACKLOG) < INIT_0) {
        perror("listen");
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * @brief Close the socket server.
 * @param ipSocketfd 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will close the socket server.
 */
int iCloseSocketServer(int *ipSocketfd) {
    // Implementation for closing the socket server
    int status = close(*ipSocketfd);
    if (status < INIT_0) {
        perror("close failed");
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Accept a client connection.
 * @param ipSocketfd 32-bit socket file descriptor pointer.
 * @return Client connected socket file descriptor on success, -1 on failure.
 * @note This function will accept a client connection.
 */
int iAcceptClientConnection(int *ipSocketfd) {  
    // Implementation for accepting a client connection
    int iNewSocket = INIT_0, iClientPort = INIT_0;
    char cArrClientIP[INET_ADDRSTRLEN]; // Buffer to store the IP string

    socklen_t sAddrLen = sizeof(sAddress);

    if ((iNewSocket = accept(*ipSocketfd, (struct sockaddr*)&sAddress, &sAddrLen)) < INIT_0) {
        perror("accept");
        return FAILURE;
    }

    // Convert the IP from binary to string format
    if(inet_ntop(AF_INET, &(sAddress.sin_addr), cArrClientIP, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop");
        return FAILURE;
    }

    // Get the port number (using ntohs to convert network byte order to host byte order)
    iClientPort = ntohs(sAddress.sin_port);

    printf("Connection accepted from %s:%d\n", cArrClientIP, iClientPort);
    return iNewSocket;
}

/**
 * @brief Read and write data to/from the client.
 * @param u8CommunicateWithClient 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will read and write data to/from the client.
 */
uint8_t u8CommunicateWithClient(int *ipSocketServerfd) {
    // Implementation for reading and writing data to/from the client
    int iConnectedSocket = INIT_0;
    ssize_t iReadCnt = INIT_0;
    char cBuffer[BUFFER_SIZE] = { INIT_0 };
    char* cpHello = "Hello from server";


    // Accept a connection from a client
    if ((iConnectedSocket = iAcceptClientConnection(ipSocketServerfd)) == FAILURE) {
        fprintf(stderr, "Failed to accept client connection\n");
        return EXIT_FAILURE;
    }

    while (1) {
    // subtract 1 for the null
    // terminator at the end
        memset(cBuffer, INIT_0, sizeof(cBuffer)); // Clear the buffer before reading
        // iReadCnt = read(iConnectedSocket, cBuffer, BUFFER_SIZE - INIT_1);  // Used for blocking read
        iReadCnt = recv(iConnectedSocket, cBuffer, BUFFER_SIZE - INIT_1, MSG_DONTWAIT); // Used for non-blocking read
        if (iReadCnt < INIT_0) {
            if(errno == EWOULDBLOCK || errno == EAGAIN) {
                // No data available, continue to the next iteration
                continue;
            } else {
                perror("recv failed");
                goto CLOSE_CONNECTION;
            }
        } else if(iReadCnt == INIT_0) {
            close(iConnectedSocket);

            printf("Client disconnected\n");
            printf("Waiting for new connection...\n");
            // Accept a connection from a client
            if ((iConnectedSocket = iAcceptClientConnection(ipSocketServerfd)) == FAILURE) {
                fprintf(stderr, "Failed to accept client connection\n");
                goto CLOSE_CONNECTION;
            } else {
                continue;
            }
        } else {
            printf("%s\n", cBuffer);
        }
        if(strcmp(cBuffer, "exitS") == INIT_0) {
            printf("Exit command received. Closing connection.\n");
            break;
        }
        send(iConnectedSocket, cpHello, strlen(cpHello), INIT_0);
    }

CLOSE_CONNECTION:
    // closing the connected socket
    close(iConnectedSocket);

    return SUCCESS;
}

int main(int argc, char const* argv[])
{
    int iServerFD = INIT_0;

    // Initialize the socket server
    if (iInitSocketServer(&iServerFD) == FAILURE) {
        fprintf(stderr, "Failed to initialize socket server\n");
        return EXIT_FAILURE;
    }

    // Accept a connection from a client and communicate with the client
    if(u8CommunicateWithClient(&iServerFD) == FAILURE) {
        fprintf(stderr, "Failed to communicate with client\n");
        return EXIT_FAILURE;
    }

    // closing the listening socket
    if (iCloseSocketServer(&iServerFD) == FAILURE) {
        fprintf(stderr, "Failed to close socket server\n");
        return EXIT_FAILURE;
    }

    return SUCCESS;
}