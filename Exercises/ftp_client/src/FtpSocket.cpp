//
// Created by marco on 25/02/2024.
//

#include <iostream>
#include <limits>
#include "../includes/FtpSocket.h"

FtpSocket::FtpSocket(SOCKET socket) {
    _socket = socket;
}

FtpSocket::~FtpSocket() {
    if (_socket != INVALID_SOCKET) {
        closesocket(_socket);
        _socket = INVALID_SOCKET;
    }
}

std::string FtpSocket::receiveResponse() const {
    char buffer[CONTROL_RESPONSE_BUFFER_SIZE] = {};

    int bytesReceived = recv(_socket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
        std::cerr << "recv failed with error: " << error << std::endl;
        return "";
    }

    std::string response(buffer, bytesReceived);
    std::cout << "Received: " << response << std::endl;
    return response;
}

int FtpSocket::receiveFileData(char *buffer) const {
    int bytesReceived = recv(_socket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
        std::cerr << "recv failed with error: " << error << std::endl;
        return SOCKET_ERROR;
    } else if (bytesReceived == 0) {
        return 0;
    }

    return bytesReceived;
}

void FtpSocket::sendCommand(const std::string &cmd) const {
    std::cout << "Sent: " << cmd;

    // ensure the command length does not exceed the maximum value for int
    if (cmd.length() > static_cast<std::string::size_type>(std::numeric_limits<int>::max())) {
        std::cerr << "Command too long to send" << std::endl;
        return;
    }

    send(_socket, cmd.c_str(), static_cast<int>(cmd.length()), 0);
}

void FtpSocket::close() {
    if (_socket != INVALID_SOCKET) {
        closesocket(_socket);
        _socket = INVALID_SOCKET;
        std::cout << "Socket connection closed" << std::endl;
    }
}

FtpSocket FtpSocket::createSocket(const std::string &serverIP, uint16_t port) {
    SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in ftpServer{};
    ftpServer.sin_family = AF_INET;
    ftpServer.sin_port = htons(port);
    ftpServer.sin_addr.s_addr = inet_addr(serverIP.c_str());

    int responseCode = connect(newSocket, (struct sockaddr *) &ftpServer, sizeof(ftpServer));
    if (responseCode != 0) {
        throw SocketConnectionFailureException("Socket connection to server failed!");
    }

    return FtpSocket(newSocket);
}