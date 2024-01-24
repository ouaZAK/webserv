// #include <stdio.h>
// #include <sys/types.h>

// int main() {
//     int fd = 42;  // Replace 42 with the file descriptor you want to add
//     int f = 4;  // Replace 42 with the file descriptor you want to add
//     fd_set read_set;
//     fd_set write_set;

//     // Initialize the set
//     FD_ZERO(&read_set);

//     // Add the file descriptor to the set
//     FD_SET(fd, &read_set);
//     FD_SET(f, &write_set);

//     // Now, you can use FD_ISSET to check if the file descriptor is in the set
//     if (FD_ISSET(fd, &read_set))
//         printf("%d IS read sset\n", fd);
//     else
//         printf("%d NOT read sset\n", fd);
//     if (FD_ISSET(fd, &write_set))
//         printf("%d IS write sset\n", fd);
//     else
//         printf("%d NOT write sset\n", fd);
//     FD_CLR(f, &read_set);
//     if (FD_ISSET(f, &write_set))
//         printf("%d IS write sset\n", f);
//     else
//         printf("%d NOT write sset\n", f);
//     if (FD_ISSET(f, &read_set))
//         printf("%d IS read sset\n", f);
//     else
//         printf("%d NOT read sset\n", f);
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSockets[MAX_CLIENTS], maxClients = MAX_CLIENTS;
    fd_set readfds;
    int maxSocket;
    int activity, i, valread;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    memset(&serverAddr, '0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    // Initialize client sockets array
    for (i = 0; i < maxClients; i++) {
        clientSockets[i] = 0;
    }

    // Main loop
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        maxSocket = serverSocket;

        // Add child sockets to set
        for (i = 0; i < maxClients; i++) {
            int socketFd = clientSockets[i];
            if (socketFd > 0) {
                FD_SET(socketFd, &readfds);
            }
            if (socketFd > maxSocket) {
                maxSocket = socketFd;
            }
        }

        // Wait for an activity on one of the sockets
        activity = select(maxSocket + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0)) {
            perror("Select error");
        }

        // Handle new connection
        if (FD_ISSET(serverSocket, &readfds)) {
            int newSocket;
            socklen_t addrlen = sizeof(clientAddr);

            if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrlen)) < 0) {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, IP is : %s, port : %d\n",
                   newSocket, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

            // Add new socket to array of sockets
            for (i = 0; i < maxClients; i++) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = newSocket;
                    break;
                }
            }
        }

        // Handle data from clients
        for (i = 0; i < maxClients; i++) {
            int socketFd = clientSockets[i];

            if (FD_ISSET(socketFd, &readfds)) 
            {
                valread = read(socketFd, buffer, BUFFER_SIZE);

                if (valread == 0) {
                    // Client disconnected
                    printf("Host disconnected, ip %s, port %d \n",
                           inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

                    // Close the socket and mark as 0 in the array
                    close(socketFd);
                    clientSockets[i] = 0;
                } 
                else {
                    // Handle data from client
                    buffer[valread] = '\0';
                    printf("Received data from client: %s\n", buffer);
                    // Add your logic to process the received data

                    // Example: Send a response back to the client
                    send(socketFd, "Hello from server", strlen("Hello from server"), 0);
                }
            }
        }
    }

    return 0;
}
