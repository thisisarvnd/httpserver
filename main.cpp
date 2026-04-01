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

    struct sockaddr_in address;
    address = {0};
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_fd, 10) < 0){
        perror("Listening failed");
        return 1;
    }

    int addrlen = sizeof(address);
    int new_connection = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    if (new_connection > 0){
        std::cout << "Connection accepted\n";
    }
    else{
        perror("Error accepting clients");
        return 1;
    }

    // Clean up before exiting
    close(server_fd);
    return 0;
}