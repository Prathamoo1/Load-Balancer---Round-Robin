#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    // Initialize Winsock
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }

    // Create socket
    SOCKET clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (clientSocket == INVALID_SOCKET) {
        cout << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }

    // Load balancer address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddress.sin_addr
    );

    // Connect to load balancer
    if (connect(
            clientSocket,
            (sockaddr*)&serverAddress,
            sizeof(serverAddress)) == SOCKET_ERROR) {

        cout << "Could not connect to load balancer!" << endl;

        closesocket(clientSocket);
        WSACleanup();

        return 1;
    }

    cout << "Connected to Load Balancer!" << endl;

    // Send message
    string message;

    cout << "Enter message: ";
    getline(cin, message);

    send(
        clientSocket,
        message.c_str(),
        static_cast<int>(message.length()),
        0
    );

    // Receive response
    char buffer[4096];

    int bytesReceived = recv(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0
    );

    if (bytesReceived > 0) {

        buffer[bytesReceived] = '\0';

        cout << "\nResponse from server: "
             << buffer << endl;

    } else {
        cout << "No response received." << endl;
    }

    // Close socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
