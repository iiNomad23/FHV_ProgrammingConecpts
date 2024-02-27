//
// Created by marco on 25/02/2024.
//

#include <sstream>
#include "../includes/FTPClient.h"
#include "../includes/enums/FTPResponseCode.h"

FTPClient::FTPClient(const std::string &serverIP, uint16_t port) {
    _controlSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    _dataSocket = INVALID_SOCKET;
    _dataPort = 0;
    _dataIP = "";

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

uint16_t FTPClient::login(const std::string &userName, const std::string &password) const {
    uint16_t responseCode;

    std::ostringstream userCommandStream;
    userCommandStream << "USER " << userName << "\r\n";
    sendCommand(_controlSocket, userCommandStream.str());
    responseCode = parseResponseCode(receiveResponse(_controlSocket));
    if (responseCode != FtpResponseCode::USER_NAME_OKAY_NEED_PASSWORD) {
        return 0;
    }

    std::ostringstream passCommandStream;
    passCommandStream << "PASS " << password << "\r\n";
    sendCommand(_controlSocket, passCommandStream.str());
    responseCode = parseResponseCode(receiveResponse(_controlSocket));
    if (responseCode != FtpResponseCode::USER_LOGGED_IN) {
        return 0;
    }

    return responseCode;
}
