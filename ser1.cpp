#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9001);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server 1 running on port 9001\n";

    while (true) {
        int client = accept(server_fd, nullptr, nullptr);

        char buffer[1024] = {};
        read(client, buffer, sizeof(buffer));

        std::cout << "Server 1 received: " << buffer << '\n';

        std::string response = "Hello from Server 1";
        send(client, response.c_str(), response.size(), 0);

        close(client);
    }

    close(server_fd);
    return 0;
}

//based on tcp
