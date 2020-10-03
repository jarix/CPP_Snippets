/*-------------------------------------------------------------------*\
  NAME
    socket_server

  DESCRIPTION
    Simple socket server example. Echos messages sent from client back.
    Run with socket_client example (socket_client.cpp)

  AUTHOR
    Jari Honkanen

\*-------------------------------------------------------------------*/

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>

 
int main(int argc, char **argv)
{
    int rc;   // Return code for networking functions

    std::cout << "Simple Socket Server.  Echos messages from clients back" << std::endl;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << "<port to listen>" << std::endl;
        return 0;  
    }

    // Create socket
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        std::cerr << "*** Error: could not create socket!" << std::endl;
        return -1;
    }
 
    // Bind to socket
    sockaddr_in listenAddr;
    listenAddr.sin_family = AF_INET;          // IPv4
    listenAddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any address
    int portNr = atoi(argv[1]);
    listenAddr.sin_port = htons(portNr);       // Convert short from host byte order to network byte order
    //inet_pton(AF_INET, "0.0.0.0", &listenAddr.sin_addr);   // Alternate way to bind to any address (0.0.0.0) 
                // inet_pton() converts character string into a network address
 
    rc = bind(listenSocket, (sockaddr *)&listenAddr, sizeof(listenAddr));
    if (rc < 0) {
        std::cerr << "*** Error: bind() failed!" << std::endl;
        return -1;
    }

    std::cout << "Waiting for connections from clients on port " << portNr << " ..." << std::endl;

    // Listen on socket
    rc = listen(listenSocket, SOMAXCONN);  // Allow max connections
    if (rc < 0) {
        std::cerr << "*** Error: listen() failed!" << std::endl;
        return -1;
    }
 
    // Wait for connections from clients
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = accept(listenSocket, (sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket < 0) {
        std::cerr << "*** Error: accept() failed!" << std::endl;
        return -1;
    }
 

    // Get info on connected client
    char hostName[NI_MAXHOST];      // host (client) name
    memset(hostName, 0, NI_MAXHOST);
    char serviceName[NI_MAXSERV];   // port (service) connected to
    memset(serviceName, 0, NI_MAXSERV);

    rc = getnameinfo((sockaddr*)&clientAddr, sizeof(clientAddr), hostName, NI_MAXHOST, serviceName, NI_MAXSERV, 0);
    if (rc == 0) {
        std::cout << "Client '" << hostName << "' connected to port '" << serviceName << "'" << std::endl;
    } else {
        inet_ntop(AF_INET, &clientAddr.sin_addr, hostName, NI_MAXHOST);
        std::cout << "Client '" << hostName << " connected to port '" << ntohs(clientAddr.sin_port) << "'" << std::endl;
    }
 
    // Close socket used for listening 
    close(listenSocket);
 
    const int RX_BUF_SIZE = 2048; 
    char rxBuf[RX_BUF_SIZE];

    // Keep Echoing any client messages back 
    while (true)
    {
        memset(rxBuf, 0, RX_BUF_SIZE);
 
        // Wait for message from client
        int nrRxBytes = recv(clientSocket, rxBuf, RX_BUF_SIZE, 0);
        if (nrRxBytes < 0) {
            std::cerr << "*** Error: recv() failed!" << std::endl;
            break;
        }
        else if (nrRxBytes == 0) {
            std::cout << "Client '" << hostName << "' has disconnected.  Bye!" << std::endl;
            break;
        }
 
        // Display message received
        std::cout << "Msg from client: '" << std::string(rxBuf, 0, nrRxBytes) << "'" << std::endl;
 
        // Send message back to client
        send(clientSocket, rxBuf, nrRxBytes+1, 0);
    }
 
    // Close client socket
    close(clientSocket);
 
    return 0;
}