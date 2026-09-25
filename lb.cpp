#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int connectToServer(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(sock,
                (sockaddr*)&serverAddress,
                sizeof(serverAddress)) < 0) {
        perror("Backend connection failed");
        close(sock);
        return -1;
    }

    return sock;
}

int main() {

    // Create load balancer socket
    int lbSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (lbSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Allow port reuse
    int opt = 1;
    setsockopt(lbSocket, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));

    // Load balancer address
    sockaddr_in lbAddress{};
    lbAddress.sin_family = AF_INET;
    lbAddress.sin_addr.s_addr = INADDR_ANY;
    lbAddress.sin_port = htons(8080);

    // Bind
    if (bind(lbSocket,
             (sockaddr*)&lbAddress,
             sizeof(lbAddress)) < 0) {
        perror("Bind failed");
        close(lbSocket);
        return 1;
    }

    // Listen
    if (listen(lbSocket, 10) < 0) {
        perror("Listen failed");
        close(lbSocket);
        return 1;
    }

    cout << "====================================\n";
    cout << "     C++ LOAD BALANCER STARTED\n";
    cout << "====================================\n";
    cout << "Listening on port 8080...\n";

    // Round-robin counter
    int currentServer = 0;

    while (true) {

        // Accept client
        int clientSocket = accept(
            lbSocket,
            nullptr,
            nullptr
        );

        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        // Select backend server
        int backendPort;

        if (currentServer == 0) {
            backendPort = 9001;
        } else {
            backendPort = 9002;
        }

        currentServer = (currentServer + 1) % 2;

        cout << "\nClient connected\n";
        cout << "Forwarding to server on port "
             << backendPort << endl;

        // Connect to selected backend
        int backendSocket = connectToServer(backendPort);

        if (backendSocket < 0) {
            const char* errorMessage =
                "Backend server unavailable";

            send(clientSocket,
                 errorMessage,
                 strlen(errorMessage),
                 0);

            close(clientSocket);
            continue;
        }

        // Receive data from client
        char buffer[4096];

        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytesReceived <= 0) {
            close(backendSocket);
            close(clientSocket);
            continue;
        }

        cout << "Request: " << buffer << endl;

        // Forward request to backend server
        send(
            backendSocket,
            buffer,
            bytesReceived,
            0
        );

        // Receive response from backend
        memset(buffer, 0, sizeof(buffer));

        int bytesReceivedFromServer = recv(
            backendSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytesReceivedFromServer > 0) {

            cout << "Response received from server\n";

            // Send response back to client
            send(
                clientSocket,
                buffer,
                bytesReceivedFromServer,
                0
            );
        }

        // Close connections
        close(backendSocket);
        close(clientSocket);

        cout << "Request completed.\n";
    }

    close(lbSocket);

    return 0;
}
