//
// Created by marco on 25/02/2024.
//

#include <sstream>
#include "FTPClient.h"

FTPClient::FTPClient(const std::string& serverIP, unsigned short port) {
    _dataSocket = INVALID_SOCKET;
    _controlSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int ret = MySocket::ftp_server_connect(_controlSocket, serverIP, port);
    if (ret != 0) {
        std::cerr << "ftp server connect failed with error: " << ret << std::endl;
        _connected = false;
    } else {
        std::cout << "successfully connected to ftp server!" << std::endl;
        _connected = true;
    }

    // Receive the welcome message
    receiveResponse(_controlSocket);
}

FTPClient::~FTPClient() {
    if (_controlSocket != INVALID_SOCKET) {
        closesocket(_controlSocket);
        _controlSocket = INVALID_SOCKET;
    }
    if (_dataSocket != INVALID_SOCKET) {
        closesocket(_dataSocket);
        _dataSocket = INVALID_SOCKET;
    }
};

bool FTPClient::isConnected() const {
    return _connected;
}

void FTPClient::login(const std::string &userName, const std::string &password) const {
    std::ostringstream userCommandStream;
    userCommandStream << "USER " << userName << "\r\n";

    // Send USER command
    sendCommand(_controlSocket, userCommandStream.str());  // Replace 'your_username' with your actual FTP username
    receiveResponse(_controlSocket);

    std::ostringstream passCommandStream;
    passCommandStream << "PASS " << password << "\r\n";

    // Send PASS command
    sendCommand(_controlSocket, passCommandStream.str());  // Replace 'your_password' with your actual FTP password
    receiveResponse(_controlSocket);
}
