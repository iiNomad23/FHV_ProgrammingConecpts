//
// Created by marco on 25/02/2024.
//

#include <iostream>
#include <limits>
#include "../includes/FtpSocket.h"

FtpSocket::FtpSocket() {
    _socket = INVALID_SOCKET;
}

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
    char buffer[1024] = {};

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