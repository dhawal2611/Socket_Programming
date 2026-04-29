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
 * @brief Get the server IP address.
 * @param cArrServerIP Buffer to store the server IP address.
 * @param size Size of the buffer.
 * @return 0 on success, -1 on failure.
 * @note This function will get the server IP address.
 */
uint8_t u8GetServerIPAddress(char *cArrServerIP, size_t size) {
    // Implementation for getting the server IP address
    char cInterfaceName[] = "enp1s0"; // Interface name
    struct ifreq sIfReq;
    struct sockaddr_in *sAddrIn;
    int iSocket = INIT_0;

    iSocket = socket(AF_INET, SOCK_DGRAM, INIT_0);
    if (iSocket < 0) {
        perror("socket failed");
        return FAILURE;
    }
    memset(&sIfReq, 0, sizeof(sIfReq));
    strncpy(sIfReq.ifr_name, cInterfaceName, sizeof(sIfReq.ifr_name) - INIT_1);
    if (ioctl(iSocket, SIOCGIFADDR, &sIfReq) < INIT_0) {
        perror("ioctl failed");
        close(iSocket);
        return FAILURE;
    }
    close(iSocket);

    // sprintf(cArrServerIP, "%s", inet_ntoa(((struct sockaddr_in *)&sIfReq.ifr_addr)->sin_addr));
    //OR
    sAddrIn = (struct sockaddr_in *)&sIfReq.ifr_addr;
    if (inet_ntop(AF_INET, &sAddrIn->sin_addr, cArrServerIP, size) == NULL) {
        perror("inet_ntop failed");
        return FAILURE;
    }
    printf("Server IP address: %s\n", cArrServerIP);

    return SUCCESS;
}

/**
 * @brief Initialize the socket server.
 * @param ipSocketfd 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will initialize the socket server.
 */
int iInitSocketServer(int *ipSocketfd) {
    // Implementation for initializing the socket server
    int iOpt = INIT_0;
    char cServerIP[64] = "127.0.0.1"; // IP address of the server

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
    
    // Get the IP address of the server
    if(u8GetServerIPAddress(cServerIP, sizeof(cServerIP)) == FAILURE) {
        fprintf(stderr, "Failed to get server IP address\n");
        return FAILURE;
    }

    printf("Starting Socket on IP: %s\n", cServerIP);
    // sAddress.sin_addr.s_addr = INADDR_ANY;
    // OR
    // Convert string IP to binary format and assign to sin_addr
    // if (inet_pton(PROTOCOL_FAMILY, SERVER_IP, &sAddress.sin_addr) <= INIT_0) { // --- IGNORE --- Start Server with hardcoded IP. Remove comment from header file as well.
    if (inet_pton(PROTOCOL_FAMILY, cServerIP, &sAddress.sin_addr) <= INIT_0) { // Use the obtained server IP address instead of hardcoded IP
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
int iAcceptClientConnection(int *ipMSocketfd) {  
    // Implementation for accepting a client connection
    int iNewSocket = INIT_0, iClientPort = INIT_0;
    char cArrClientIP[INET_ADDRSTRLEN]; // Buffer to store the IP string

    int activity, sd;
    int max_sd;

    socklen_t sAddrLen = sizeof(sAddress);

    // Clear the socket set
    FD_ZERO(&readfds);

    // Add master socket to set
    FD_SET(*ipMSocketfd, &readfds);
    max_sd = *ipMSocketfd;

    // Add child sockets to set
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sd = iClientSockets[i];
        if (sd > 0) {
            FD_SET(sd, &readfds);
        }
        if (sd > max_sd) {
            max_sd = sd;
        }
    }
    // Wait indefinitely for activity on one of the sockets
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR)) {
        printf("Select error\n");
    }

    // Handle incoming connection on master socket
    if (FD_ISSET(*ipMSocketfd, &readfds)) {
        if ((iNewSocket = accept(*ipMSocketfd, (struct sockaddr *)&sAddress, (socklen_t *)&sAddrLen)) < 0) {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }

        // Convert the IP from binary to string format
        if(inet_ntop(AF_INET, &(sAddress.sin_addr), cArrClientIP, INET_ADDRSTRLEN) == NULL) {
            perror("inet_ntop");
            return FAILURE;
        }

        // Get the port number (using ntohs to convert network byte order to host byte order)
        iClientPort = ntohs(sAddress.sin_port);

        printf("Connection accepted from %s:%d\n", cArrClientIP, iClientPort);

        // Add new socket to array
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (iClientSockets[i] == 0) {
                iClientSockets[i] = iNewSocket;
                printf("Adding to list of sockets as %d\n", i);
                break;
            }
        }
    }

    return iNewSocket;
}

/**
 * @brief Read and write data to/from the client.
 * @param u8CommunicateWithClient 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will read and write data to/from the client.
 */
uint8_t u8CommunicateWithClient(int *ipSocketServerFD) {
    // Implementation for reading and writing data to/from the client
    int iConnectedSocket = INIT_0;
    ssize_t iReadCnt = INIT_0;
    char cBuffer[BUFFER_SIZE] = { INIT_0 };
    char* cpHello = "Hello from server";
    socklen_t sAddrLen = sizeof(sAddress);

    while (1) {
        // Accept a connection from a client
        if ((iConnectedSocket = iAcceptClientConnection(ipSocketServerFD)) == FAILURE) {
            fprintf(stderr, "Failed to accept client connection\n");
            return EXIT_FAILURE;
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (iClientSockets[i] != 0) {
                if (FD_ISSET(iClientSockets[i], &readfds)) { 
                    // subtract 1 for the null
                    // terminator at the end
                    memset(cBuffer, INIT_0, sizeof(cBuffer)); // Clear the buffer before reading
                    iReadCnt = read(iClientSockets[i], cBuffer, BUFFER_SIZE - INIT_1);  // Used for blocking read
                    // iReadCnt = recv(iClientSockets[i], cBuffer, BUFFER_SIZE - INIT_1, MSG_DONTWAIT); // Used for non-blocking read
                    if (iReadCnt < INIT_0) {
                        if(errno == EWOULDBLOCK || errno == EAGAIN) {
                            // No data available, continue to the next iteration
                            continue;
                        } else {
                            perror("recv failed");
                            goto RETURN_CLOSE_CONNECTION;
                        }
                    } else if(iReadCnt == INIT_0) {
                        // Somebody disconnected, get his details and print 
                        getpeername(iClientSockets[i], (struct sockaddr*)&sAddress, (socklen_t*)&sAddrLen); 
                        printf("Host disconnected, ip %s, port %d \n", inet_ntoa(sAddress.sin_addr), ntohs(sAddress.sin_port)); 
                        close(iClientSockets[i]);
                        iClientSockets[i] = INIT_0;

                        printf("Client disconnected\n");
                        printf("Waiting for new connection...\n");
                    } else {
                        printf("%s\n", cBuffer);
                    }
                    if(strcmp(cBuffer, "exitS") == INIT_0) {
                        printf("Exit command received. Closing connection.\n");
                        break;
                    }
                    send(iClientSockets[i], cpHello, strlen(cpHello), INIT_0);
                }
            }
        }
    }

RETURN_CLOSE_CONNECTION:
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