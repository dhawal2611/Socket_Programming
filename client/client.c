/**
 * @file        client.c
 * @brief       Socket client code
 * @details     Create a socket client to get connected 
 *              to a server and communicate with it.
 * @author      Lad Dhawal Umesh
 * @developedBy Dhawal Umesh Lad
 * @date        2026-04-14
 * @version     1.0.0
 * @copyright   (c) 2026 Dhawal Umesh Lad. All rights reserved.
 * Note: This code is an original implementation by Lad Dhawal Umesh. 
 * Certain algorithms or logic patterns were adapted from publicly 
 * available online resources.
 */

// Include Header
#include "client.h"

// Functions

/**
 * @brief Initialize the socket client.
 * @param ipSocketClifd 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will initialize the socket client.
 */
int iInitSocketClient(int *ipSocketClifd) {
    // Implementation for initializing the socket client

    // Create socket file descriptor
    *ipSocketClifd = socket(PROTOCOL_FAMILY, SOCKET_TYPE, PROTOCOL);
    if (*ipSocketClifd < INIT_0) {
        perror("socket failed");
        return FAILURE;
    }

    sAddress.sin_family = PROTOCOL_FAMILY;
    sAddress.sin_port = htons(CLIENT_PORT);
    
    // sAddress.sin_addr.s_addr = INADDR_ANY;
    // OR
    // Convert string IP to binary format and assign to sin_addr
    if (inet_pton(PROTOCOL_FAMILY, SERVER_IP, &sAddress.sin_addr) <= INIT_0) {
        perror("Invalid address/Address not supported");
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * @brief Close the socket server.
 * @param ipSocketClifd 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will close the socket server.
 */
int iCloseSocketClient(int *ipSocketClifd) {
    // Implementation for closing the socket client
    int status = close(*ipSocketClifd);
    if (status < INIT_0) {  
        perror("close failed");
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * @brief Connect to the server.
 * @param ipSocketClifd 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will connect to the server.
 */
int iConnectToServer(int *ipSocketClifd) {
    // Implementation for connecting to the server
    if (connect(*ipSocketClifd, (struct sockaddr*)&sAddress, sizeof(sAddress)) < INIT_0) {
        perror("Connection Failed");
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * @brief Read and write data to/from the server.
 * @param vSocketClifd 32-bit socket file descriptor pointer.
 * @return 0 on success, -1 on failure.
 * @note This function will read and write data to/from the server.
 */
uint8_t vCommunicateWithServer(int *ipSocketClifd) {
    // Implementation for reading and writing data to/from the server
    char cBuffer[BUFFER_SIZE] = { INIT_0 };
    // char* cpHello = "Hello from client";
    ssize_t iRetStatus;

    while(1) {
        // Get user input
        printf("Enter message to send to server (type 'exit' to quit): ");
        if (fgets(cBuffer, sizeof(cBuffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            break;
        }

        // Remove newline character from the end of the input
        cBuffer[strcspn(cBuffer, "\n")] = '\0';

        // Check for exit command
        if (strcmp(cBuffer, "exit") == INIT_0) {
            printf("Exit command received. Closing connection.\n");
            break;
        }

        // Send message to server
        if ((iRetStatus = send(*ipSocketClifd, cBuffer, strlen(cBuffer), INIT_0)) < INIT_0) {
            perror("send failed");
            return FAILURE;
        }

        printf("Message sent\n");
        
        memset(cBuffer, INIT_0, sizeof(cBuffer)); // Clear the buffer after sending
        iRetStatus = read(*ipSocketClifd, cBuffer, BUFFER_SIZE - INIT_1);
        if (iRetStatus < INIT_0) {
            perror("read failed");
            return FAILURE;
        }
        printf("%s\n", cBuffer);
    }
    return SUCCESS;
}

int main(int argc, char const* argv[])
{
    int iClientFD = INIT_0;

    // Initialize the socket client
    if (iInitSocketClient(&iClientFD) == FAILURE) {
        fprintf(stderr, "Failed to initialize socket client\n");
        return FAILURE;
    }

    // Connect to the server
    if (iConnectToServer(&iClientFD) == FAILURE) {
        fprintf(stderr, "Failed to connect to server\n");
        return FAILURE;
    }

    // Communicate with the server
    if (vCommunicateWithServer(&iClientFD) == FAILURE) {
        fprintf(stderr, "Failed to communicate with server\n");
    }

    // closing the connected socket
    if (iCloseSocketClient(&iClientFD) == FAILURE) {
        fprintf(stderr, "Failed to close socket client\n");
        return FAILURE;
    }

    return SUCCESS;
}
