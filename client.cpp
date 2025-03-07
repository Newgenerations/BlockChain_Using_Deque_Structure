#include "client.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

Client::Client(const std::string &server_ip, int port)
    : server_ip(server_ip), port(port)
{

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }
}

void Client::sendRequest(const std::string &request)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Connection failed!" << std::endl;
        return;
    }

    send(socket_fd, request.c_str(), request.size(), 0);

    char buffer[1024] = {0};
    int valread = read(socket_fd, buffer, sizeof(buffer));
    if (valread > 0)
    {
        std::cout << "Server: " << buffer << std::endl;
    }

    close(socket_fd);
}
