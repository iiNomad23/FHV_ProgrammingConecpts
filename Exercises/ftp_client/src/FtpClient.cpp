//
// Created by marco on 25/02/2024.
//

#include <sstream>
#include <fstream>
#include <chrono>
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

void FtpClient::get(const std::string &fileName) {
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

        size_t fileSize = getFileSize(fileName);

        _controlSocket.sendCommand("RETR " + fileName + "\r\n");
        responseCode = parseResponseCode(_controlSocket.receiveResponse());
        if (responseCode != FtpServerResponseCode::FILE_STATUS_OKAY) {
            if (responseCode == FtpServerResponseCode::FILE_NOT_FOUND) {
                throw SocketDataFailureException(
                        "Error: File not found with command response status " + std::to_string(responseCode)
                );
            } else {
                throw SocketDataFailureException(
                        "Error: data connection failed with command response status " + std::to_string(responseCode)
                );
            }
        }

        std::ofstream outFile(fileName, std::ofstream::binary);
        if (!outFile.is_open()) {
            throw SocketDataFailureException("Failed to open local file for writing: " + fileName);
        }

        size_t received = 0;
        auto lastUpdateTime = std::chrono::steady_clock::now();

        char buffer[4096];
        int bytesRead;
        while ((bytesRead = dataSocket.receiveFileData(buffer)) > 0) {
            outFile.write(buffer, bytesRead);

            // for progress bar
            auto now = std::chrono::steady_clock::now();
            auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();
            if (timeSinceLastUpdate > 10) {
                displayProgress(received, fileSize);
                lastUpdateTime = now;
            }
        }

        displayProgress(fileSize, fileSize);
        std::cerr << std::endl;
        std::cerr << std::endl;

        outFile.close();
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

size_t FtpClient::getFileSize(const std::string& fileName) {
    _controlSocket.sendCommand("SIZE " + fileName + "\r\n");

    std::string response = _controlSocket.receiveResponse();
    int16_t responseCode = parseResponseCode(response);
    if (responseCode != FILE_STATUS) {
        std::cerr << "Failed to get file size. Response: " << response << std::endl;
        return 0;
    }

    size_t fileSize = std::stoll(response.substr(4));
    return fileSize;
}

void FtpClient::displayProgress(size_t received, size_t total) {
    const int barWidth = 50;
    std::cout << "[";
    int pos = barWidth * received / total;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << (received * 100 / total) << " %\r" << std::flush;
}


