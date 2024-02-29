//
// Created by marco on 25/02/2024.
//

#include <sstream>
#include <fstream>
#include "../includes/FtpClient.h"
#include "../includes/enums/FtpServerResponseCode.h"
#include "../includes/exeptions/LoginFailureException.h"
#include "../includes/exeptions/SocketDataFailureException.h"

FtpClient::FtpClient(const std::string &serverIP, const FtpSocket &controlSocket) : _controlSocket(controlSocket) {
    _ftpServerIp = serverIP;
}

FtpClient::~FtpClient() {
    _controlSocket.close();
    WSACleanup();
}

void FtpClient::login(const std::string &userName, const std::string &password) const {
    int16_t responseCode;

    _controlSocket.sendCommand("USER " + userName + "\r\n");
    responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
    if (responseCode != FtpServerResponseCode::USER_NAME_OKAY_NEED_PASSWORD) {
        throw LoginFailureException(
                "Error: login failed with command response status " + std::to_string(responseCode)
        );
    }

    _controlSocket.sendCommand("PASS " + password + "\r\n");
    responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
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
    std::string pasvResponse = _controlSocket.receiveResponse(true);
    responseCode = parseResponseCode(pasvResponse);
    if (responseCode != FtpServerResponseCode::ENTERING_PASSIVE_MODE) {
        throw SocketDataFailureException(
                "Error: data connection failed with command response status " + std::to_string(responseCode)
        );
    }

    try {
        uint16_t port = parsePasvResponse(pasvResponse);
        FtpSocket dataSocket = FtpSocket::createSocket(_ftpServerIp, port);

        _controlSocket.sendCommand("LIST\r\n");
        responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
        if (responseCode != FtpServerResponseCode::FILE_STATUS_OKAY) {
            throw SocketDataFailureException(
                    "Error: data connection failed with command response status " + std::to_string(responseCode)
            );
        }

        std::cout << "Directory listing:" << std::endl;
        std::cout << dataSocket.receiveResponse(false) << std::endl;
        // dataSocket closes in ~FtpSocket

        responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
        if (responseCode != FtpServerResponseCode::CLOSING_DATA_CONNECTION) {
            throw SocketDataFailureException(
                    "Error: data connection failed with command response status " + std::to_string(responseCode)
            );
        }
    } catch (const ParsePasvFailureException &e) {
        throw SocketDataFailureException(e.what());
    } catch (const SocketDataFailureException &e) {
        throw;
    }
}

void FtpClient::get(const std::string &fileName) {
    int16_t responseCode;

    _controlSocket.sendCommand("PASV\r\n");
    std::string pasvResponse = _controlSocket.receiveResponse(true);
    responseCode = parseResponseCode(pasvResponse);
    if (responseCode != FtpServerResponseCode::ENTERING_PASSIVE_MODE) {
        throw SocketDataFailureException(
                "Error: data connection failed with command response status " + std::to_string(responseCode)
        );
    }

    try {
        uint16_t port = parsePasvResponse(pasvResponse);
        FtpSocket dataSocket = FtpSocket::createSocket(_ftpServerIp, port);

        size_t fileSize = getFileSize(fileName);

        _controlSocket.sendCommand("RETR " + fileName + "\r\n");
        responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
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

        dataSocket.receiveFileData(fileName, fileSize);
        // dataSocket closes in ~FtpSocket

        responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
        if (responseCode != FtpServerResponseCode::CLOSING_DATA_CONNECTION) {
            throw SocketDataFailureException(
                    "Error: data connection failed with command response status " + std::to_string(responseCode)
            );
        }
    } catch (const ParsePasvFailureException &e) {
        throw SocketDataFailureException(e.what());
    } catch (const SocketDataFailureException &e) {
        throw;
    }
}

void FtpClient::setAsciiMode() {
    _controlSocket.sendCommand("TYPE A\r\n");
    int16_t responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
    if (responseCode != FtpServerResponseCode::COMMAND_OKAY) {
        std::cerr << "Switching to ascii mode failed" << std::endl;
    }
}

void FtpClient::setBinaryMode() {
    _controlSocket.sendCommand("TYPE I\r\n");
    int16_t responseCode = parseResponseCode(_controlSocket.receiveResponse(true));
    if (responseCode != FtpServerResponseCode::COMMAND_OKAY) {
        std::cerr << "Switching to binary mode failed" << std::endl;
    }
}

size_t FtpClient::getFileSize(const std::string &fileName) {
    _controlSocket.sendCommand("SIZE " + fileName + "\r\n");

    std::string response = _controlSocket.receiveResponse(true);
    int16_t responseCode = parseResponseCode(response);
    if (responseCode != FtpServerResponseCode::FILE_STATUS) {
        throw SocketDataFailureException("Error: File does not exist");
    }

    size_t fileSize = std::stoll(response.substr(4));
    return fileSize;
}

int16_t FtpClient::parseResponseCode(const std::string &response) {
    try {
        return static_cast<int16_t>(std::stoi(response.substr(0, 3)));
    } catch (const std::exception &e) {
        std::cerr << "Error parsing response code: " << e.what() << std::endl;
        return 0;
    }
}

uint16_t FtpClient::parsePasvResponse(const std::string &response) {
    std::regex pasvPattern(R"(\((\d+),(\d+),(\d+),(\d+),(\d+),(\d+)\))");
    std::smatch matches;

    if (std::regex_search(response, matches, pasvPattern)) {
        if (matches.size() == 7) {
            int p1 = std::stoi(matches[5].str());
            int p2 = std::stoi(matches[6].str());
            return (p1 * 256) + p2;
        } else {
            throw ParsePasvFailureException("Failed to parse PASV response: " + response);
        }
    } else {
        throw ParsePasvFailureException("PASV response does not match expected format: " + response);
    }
}

void FtpClient::displayCommands() {
    std::cout << std::endl;
    std::cout << "Command List:" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "ls: List directory contents" << std::endl;
    std::cout << "get <filename>: Download a file" << std::endl;
    std::cout << "ascii: Set transfer mode to ASCII" << std::endl;
    std::cout << "binary: Set transfer mode to binary" << std::endl;
    std::cout << "exit: closes the connection and the process" << std::endl;
    std::cout << std::endl;
}