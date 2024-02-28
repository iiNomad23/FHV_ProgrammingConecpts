//
// Created by marco on 25/02/2024.
//

#include <sstream>
#include "../includes/FtpClient.h"
#include "../includes/enums/FtpServerResponseCode.h"
#include "../includes/exeptions/LoginFailureException.h"

FtpClient::FtpClient(const std::string &serverIP, uint16_t port) : _controlSocket(FtpSocket::createSocket(serverIP, port)) {
    _connected = true;

    (void)_controlSocket.receiveResponse();

    _dataSocket = FtpSocket();
    _dataPort = 0;
    _dataIP = "";
}

FtpClient::~FtpClient() {
    _dataSocket.close();
    _controlSocket.close();
    WSACleanup();
}

bool FtpClient::isConnected() const {
    return _connected;
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
    _dataSocket.close();
    _controlSocket.close();
    _connected = false;
    WSACleanup();
}

int16_t FtpClient::ls() {
    if (!_connected) {
        std::cerr << "Not connected to any FTP server." << std::endl;
        return -1;
    }

    std::ostringstream passCommandStream;
    passCommandStream << "PASV \r\n";
    _controlSocket.sendCommand(passCommandStream.str());
    std::string pasvResponse = _controlSocket.receiveResponse();

    std::string ip;
    uint16_t port;

    return 0;

//    try {
//        parsePasvResponse(pasvResponse, ip, port);
//
//        // Open data connection
//        _dataSocket = createDataConnection(ip, port); // You need to implement createDataConnection
//
//        // Send LIST command
//        sendCommand(_controlSocket, "LIST\r\n");
//        std::string listResponse = receiveResponse(_dataSocket); // Read the directory listing from the data connection
//
//        std::cout << "Directory listing:\n" << listResponse << std::endl;
//
//        // Close data connection
//        closesocket(_dataSocket);
//        _dataSocket = INVALID_SOCKET;
//
//        // Read and discard the final response from the control connection (e.g., "226 Transfer complete")
//        std::string finalResponse = receiveResponse(_controlSocket);
//    } catch (const ParsePasvFailureException& e) {
//        std::cerr << e.what() << std::endl;
//        return -1;
//    }
}

int16_t FtpClient::get() {
    throw LoginFailureException("Error: command response status" + std::to_string(-1));
}

int16_t FtpClient::ascii() {
    throw LoginFailureException("Error: command response status" + std::to_string(-1));
}

int16_t FtpClient::binary() {
    throw LoginFailureException("Error: command response status" + std::to_string(-1));
}
