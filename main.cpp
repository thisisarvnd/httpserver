#include <iostream>
#include <sys/socket.h> // Core socket functions
#include <netinet/in.h> // Internet address family (AF_INET)
#include <unistd.h>     // For close()
#include <cstdio>       // For perror()

int main() {
    // 1. Create the socket
    // AF_INET: IPv4, SOCK_STREAM: TCP, 0: Default protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    std::cout << "Socket created successfully! FD: " << server_fd << std::endl;

    // --- Future steps (Bind, Listen, Accept) go here ---

    // create socket that listen for ipv4 on any interface.
    // sockaddr is the generic struct that unix bind() returns or expects
    // you send this sockaddr struct to let the kernel know what and where to listen
    // sockaddr_in is basically ipv4 struct of sockaddr which is generic (kernel supports a lot of comms)
    // we cast this ipv4 struct to a generic struct, which is what the kernel expects


    struct sockaddr_in address;
    address = {0};
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    

    //bind the socket

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // listen at this socket
    if (listen(server_fd, 10) < 0){
        perror("Listening failed");
        return 1;
    }

    // accept new connections and have a separate socket for each connection
    // this ensures main socket stays listening for new connections
    int addrlen = sizeof(address);
    int new_connection = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    if (new_connection > 0){
        std::cout << "Connection accepted\n";
    }
    else{
        perror("Error accepting clients");
        return 1;
    }

    // read the header
    char buffer[1024] = {0};
    int valread = read(new_connection, buffer, 1024);

    std::cout << "\n\n\n---Request---\n" << buffer << "-----\n" << std::endl;

    // send response

    std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";

    int valsend = send(new_connection, hello.c_str(), hello.size(), 0);

    std::cout << "response sent" << valsend << " " << sizeof(hello) << std::endl;

    // I created a bug here. corrected it now
    // std::string is a class so use its methods to retrieve the value

    // close client connection
    close(new_connection);

    // Clean up before exiting
    close(server_fd);
    return 0;
}