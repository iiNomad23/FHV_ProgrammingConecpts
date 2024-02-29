//
// Created by marco on 25/02/2024.
//

#include <iostream>
#include <limits>
#include <chrono>
#include <fstream>
#include "../includes/FtpSocket.h"

FtpSocket::FtpSocket(SOCKET socket) {
    _socket = socket;
}

FtpSocket::~FtpSocket() {
    close();
}

std::string FtpSocket::receiveResponse(bool isControlResponse) const {
    std::string response;

    char buffer[RESPONSE_BUFFER_SIZE] = {};
    if (isControlResponse) {
        response.append(buffer, receiveResponseChunk(buffer, RESPONSE_BUFFER_SIZE));
    } else {
        int bytesRead;
        while ((bytesRead = receiveResponseChunk(buffer, RESPONSE_BUFFER_SIZE)) > 0) {
            response.append(buffer, bytesRead);
        }
    }

    std::cout << "Received: " << response << std::endl;
    return response;
}

int FtpSocket::receiveResponseChunk(char *buffer, uint16_t bufferSize) const {
    int bytesReceived = recv(_socket, buffer, bufferSize, 0);
    if (bytesReceived == SOCKET_ERROR) {
        int error = WSAGetLastError();
        std::cerr << "recv failed with error: " << error << std::endl;
        return SOCKET_ERROR;
    } else if (bytesReceived == 0) {
        return 0;
    }

    return bytesReceived;
}

void FtpSocket::receiveFileData(const std::string &fileName, const size_t fileSize) {
    std::ofstream outFile(fileName, std::ofstream::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open local file for writing: " << fileName << std::endl;
        return;
    }

    size_t receivedBytes = 0;
    auto lastUpdateTimeMs = std::chrono::steady_clock::now();

    char buffer[FILE_BUFFER_SIZE];
    int bytesRead;
    while ((bytesRead = receiveResponseChunk(buffer, FILE_BUFFER_SIZE)) > 0) {
        outFile.write(buffer, bytesRead);

        // for progress bar
        auto nowMs = std::chrono::steady_clock::now();
        auto timeSinceLastUpdateMs = std::chrono::duration_cast<std::chrono::milliseconds>
                (nowMs - lastUpdateTimeMs).count();

        receivedBytes += bytesRead;
        if (timeSinceLastUpdateMs > PROGRESS_BAR_UPDATE_INTERVAL_MS) {
            displayProgress(receivedBytes, fileSize);
            lastUpdateTimeMs = nowMs;
        }
    }

    if (bytesRead >= 0) {
        displayProgress(fileSize, fileSize);
    } else {
        std::cerr << "Download canceled" << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;

    outFile.close();
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

void FtpSocket::displayProgress(size_t received, size_t total) {
    int pos = int(PROGRESS_BAR_WIDTH * received / total);

    std::cout << "[";
    for (int i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << (received * 100 / total) << " %\r" << std::flush;
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