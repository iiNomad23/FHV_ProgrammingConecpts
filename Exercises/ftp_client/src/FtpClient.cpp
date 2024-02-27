//
// Created by marco on 25/02/2024.
//

#include <sstream>
#include "../includes/FtpClient.h"
#include "../includes/enums/FtpServerResponseCode.h"
#include "../includes/exeptions/ResponseFailureException.h"

FtpClient::FtpClient(const std::string &serverIP, uint16_t port) {
    _controlSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    _dataSocket = INVALID_SOCKET;
    _dataPort = 0;
    _dataIP = "";

    int responseCode = FtpSocket::ftpServerConnect(_controlSocket, serverIP, port);
    if (responseCode != 0) {
        std::cerr << "ftp server connect failed with error: " << responseCode << std::endl;
        _connected = false;
    } else {
        std::cout << "successfully connected to ftp server!" << std::endl;
        _connected = true;
    }

    receiveResponse(_controlSocket);
}

FtpClient::~FtpClient() {
    if (_controlSocket != INVALID_SOCKET) {
        closesocket(_controlSocket);
        _controlSocket = INVALID_SOCKET;
    }
    if (_dataSocket != INVALID_SOCKET) {
        closesocket(_dataSocket);
        _dataSocket = INVALID_SOCKET;
    }
}

bool FtpClient::isConnected() const {
    return _connected;
}

void FtpClient::login(const std::string &userName, const std::string &password) const {
    int16_t responseCode;

    std::ostringstream userCommandStream;
    userCommandStream << "USER " << userName << "\r\n";
    sendCommand(_controlSocket, userCommandStream.str());
    responseCode = parseResponseCode(receiveResponse(_controlSocket));
    if (responseCode != FtpServerResponseCode::USER_NAME_OKAY_NEED_PASSWORD) {
        throw ResponseFailureException("Error: command response status" + std::to_string(responseCode));
    }

    std::ostringstream passCommandStream;
    passCommandStream << "PASS " << password << "\r\n";
    sendCommand(_controlSocket, passCommandStream.str());
    responseCode = parseResponseCode(receiveResponse(_controlSocket));
    if (responseCode != FtpServerResponseCode::USER_LOGGED_IN) {
        throw ResponseFailureException("Error: command response status" + std::to_string(responseCode));
    }
}

int16_t FtpClient::close() {
    throw ResponseFailureException("Error: command response status" + std::to_string(-1));
}

int16_t FtpClient::ls() {
    throw ResponseFailureException("Error: command response status" + std::to_string(-1));
}

int16_t FtpClient::get() {
    throw ResponseFailureException("Error: command response status" + std::to_string(-1));
}

int16_t FtpClient::ascii() {
    throw ResponseFailureException("Error: command response status" + std::to_string(-1));
}

int16_t FtpClient::binary() {
    throw ResponseFailureException("Error: command response status" + std::to_string(-1));
}
