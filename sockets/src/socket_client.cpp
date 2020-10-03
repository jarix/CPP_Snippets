/*-------------------------------------------------------------------*\
  NAME
    socket_client

  DESCRIPTION
    Simple socket client example. Send messages to server and waits for
    them to be echoed back from the server. 
    Run together with socket_server example (socket_server.cpp)

  AUTHOR
    Jari Honkanen

\*-------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char **argv)
{
    int rc;  // Return code for networking functions

    std::cout << "Simple Socket Client. Send message to server and display response" << std::endl;

    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <server address> <port>" << std::endl;
        return 0;  
    }   

    //	Create a socket
    int sendSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sendSocket < 0) {
        std::cerr << "*** Error: could not create socket!" << std::endl;
        return -1;
    }

    // Define address and connect to socket
    int portNr = atoi(argv[2]);
    std::string ipAddress = argv[1];
    std::cout << "Connecting to server: '" << ipAddress << ":" << portNr << "'" << std::endl;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;       // IPv4
    serverAddr.sin_port = htons(portNr);   // Convert short from host byte order to network byte order
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);   // convert character string into a network address

    //	Connect to the server on the socket
    rc = connect(sendSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (rc < 0) {
        std::cerr << "*** Error: connect() failed!" << std::endl;
        return -1;
    }

    //	Send messages and display server response
    const int RX_BUF_SIZE = 2048; 
    char rxBuf[RX_BUF_SIZE];
    std::string userInput;

    do {
        // Ask for Message to send
        std::cout << "> ";
        std::getline(std::cin, userInput);

        // Send message over socket
        rc = send(sendSocket, userInput.c_str(), userInput.size() + 1, 0);
        if (rc < 0) {
            std::cerr << "*** Error: send() failed" << std::endl;
            continue;
        }

        // Wait for response from server
        memset(rxBuf, 0, RX_BUF_SIZE);
        int nrRxBytes = recv(sendSocket, rxBuf, RX_BUF_SIZE, 0);
        if (nrRxBytes < 0) {
            std::cerr << "*** Error: recv() failed!" << std::endl;

        } else {
            // Display server response
            std::cout << "Server response: '" << std::string(rxBuf, nrRxBytes) << "'" << std::endl;
        }

    } while(true);

    //	Close socket
    close(sendSocket);

    return 0;
}