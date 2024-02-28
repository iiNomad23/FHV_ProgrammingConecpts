//
// Created by marco on 25/02/2024.
//

#include <sstream>
#include "../includes/FtpClient.h"
#include "../includes/enums/FtpServerResponseCode.h"
#include "../includes/exeptions/LoginFailureException.h"
#include "../includes/exeptions/SocketDataFailureException.h"

FtpClient::FtpClient(const std::string &serverIP, uint16_t port) : _controlSocket(
        FtpSocket::createSocket(serverIP, port)) {
    _ftpServerIp = serverIP;
    (void) _controlSocket.receiveResponse();
}

FtpClient::~FtpClient() {
    _controlSocket.close();
    WSACleanup();
}

void FtpClient::login(const std::string &userName, const std::string &password) const {
    int16_t responseCode;

    std::ostringstream userCommandStream;
    userCommandStream << "USER " << userName << "\r\n";
    _controlSocket.sendCommand(userCommandStream.str());

    responseCode = parseResponseCode(_controlSocket.receiveResponse());
    if (responseCode != FtpServerResponseCode::USER_NAME_OKAY_NEED_PASSWORD) {
        throw LoginFailureException(
                "Error: login failed with command response status " + std::to_string(responseCode)
        );
    }

    std::ostringstream passCommandStream;
    passCommandStream << "PASS " << password << "\r\n";
    _controlSocket.sendCommand(passCommandStream.str());

    responseCode = parseResponseCode(_controlSocket.receiveResponse());
    if (responseCode != FtpServerResponseCode::USER_LOGGED_IN) {
        throw LoginFailureException(
                "Error: login failed with command response status " + std::to_string(responseCode)
        );
    }
}

void FtpClient::close() {
    _controlSocket.close();
    WSACleanup();
}

void FtpClient::ls() {
    int16_t responseCode;

    _controlSocket.sendCommand("PASV\r\n");
    std::string pasvResponse = _controlSocket.receiveResponse();
    responseCode = parseResponseCode(pasvResponse);
    if (responseCode != FtpServerResponseCode::ENTERING_PASSIVE_MODE) {
        throw SocketDataFailureException(
                "Error: data connection failed with command response status " + std::to_string(responseCode)
        );
    }

    try {
        uint16_t port = parsePasvResponse(pasvResponse, port);
        FtpSocket dataSocket = FtpSocket::createSocket(_ftpServerIp, port);

        _controlSocket.sendCommand("LIST\r\n");
        responseCode = parseResponseCode(_controlSocket.receiveResponse());
        if (responseCode != FtpServerResponseCode::FILE_STATUS_OKAY) {
            throw SocketDataFailureException(
                    "Error: data connection failed with command response status " + std::to_string(responseCode)
            );
        }

        std::cout << "Directory listing:\n" << std::endl;
        (void) dataSocket.receiveResponse();
        dataSocket.close();

        responseCode = parseResponseCode(_controlSocket.receiveResponse());
        if (responseCode != FtpServerResponseCode::CLOSING_DATA_CONNECTION) {
            throw SocketDataFailureException(
                    "Error: data connection failed with command response status " + std::to_string(responseCode)
            );
        }
    } catch (const ParsePasvFailureException &e) {
        throw SocketDataFailureException(e.what());
    }
}

void FtpClient::get() {
    throw LoginFailureException("Error: command response status" + std::to_string(-1));
}

void FtpClient::setAsciiMode() {
    _controlSocket.sendCommand("TYPE A\r\n");
    int16_t responseCode = parseResponseCode(_controlSocket.receiveResponse());
    if (responseCode != FtpServerResponseCode::COMMAND_OKAY) {
        std::cerr << "Switching to ascii mode failed" << std::endl;
    }
}

void FtpClient::setBinaryMode() {
    _controlSocket.sendCommand("TYPE I\r\n");
    int16_t responseCode = parseResponseCode(_controlSocket.receiveResponse());
    if (responseCode != FtpServerResponseCode::COMMAND_OKAY) {
        std::cerr << "Switching to binary mode failed" << std::endl;
    }
}
