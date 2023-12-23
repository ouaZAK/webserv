#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
std::string readImageFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening image file: " << filename << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string readHtmlFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening HTML file: " << filename << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void sendResponse(int clientSocket, const std::string& header, const std::string& content) {
    std::string response = header + content;

    // Send HTTP response
    send(clientSocket, response.c_str(), response.length(), 0);
}

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket.\n";
        return -1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);  // Port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket.\n";
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections.\n";
        close(serverSocket);
        return -1;
    }

    while (true) {
        std::cout << "Server listening on port 80...\n";

        // Accept a connection
        sockaddr_in clientAddress;
        socklen_t clientAddrSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection.\n";
            close(serverSocket);
            return -1;
        }

        // Read the HTTP request
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error reading HTTP request.\n";
            close(clientSocket);
            continue;
        }

        // Parse the HTTP request to determine the requested resource
        std::string request(buffer, bytesRead);
        size_t pos = request.find("GET ");
        if (pos == std::string::npos) {
            std::cerr << "Invalid HTTP request.\n";
            close(clientSocket);
            continue;
        }

        size_t endPos = request.find(" ", pos + 4);
        if (endPos == std::string::npos) {
            std::cerr << "Invalid HTTP request.\n";
            close(clientSocket);
            continue;
        }

        std::string requestedResource = request.substr(pos + 4, endPos - 4);

        // Read the requested resource and send the corresponding response
        if (requestedResource == "/index.html") {
            std::string htmlContent = readHtmlFile("index.html");
            std::string responseHeader = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(htmlContent.length()) + "\r\nContent-Type: text/html\r\n\r\n";
            sendResponse(clientSocket, responseHeader, htmlContent);
        } else if (requestedResource == "/11.png") {
            std::string imageContent = readImageFile("11.png");
            std::string responseHeader = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(imageContent.length()) + "\r\nContent-Type: image/png\r\n\r\n";
            sendResponse(clientSocket, responseHeader, imageContent);
        } else {
            std::cerr << "Requested resource not found: " << requestedResource << "\n";
            close(clientSocket);
            continue;
        }

        // Close the socket for this connection
        close(clientSocket);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}

