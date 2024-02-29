//
// Created by marco on 11/02/2024.
//

#include "includes/FtpClient.h"
#include "includes/enums/FtpCommand.h"
#include "includes/enums/FtpServerResponseCode.h"
#include "includes/exeptions/LoginFailureException.h"
#include "includes/exeptions/SocketDataFailureException.h"

void waitForEnter() {
    std::cout << "Press Enter to exit..." << std::endl;
    std::string line;
    std::getline(std::cin, line);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize WinSock" << std::endl;
        waitForEnter();
        return 0;
    }

    const std::string serverIP = "127.0.0.1";
    const uint16_t port = 21;

    try {
        FtpSocket controlSocket = FtpSocket::createSocket(serverIP, port);
        int16_t responseCode = FtpClient::parseResponseCode(controlSocket.receiveResponse(true));
        if (responseCode != FtpServerResponseCode::SERVICE_READY) {
            throw SocketConnectionFailureException(
                    "FTP service not ready " + std::to_string(responseCode)
            );
        }

        FtpClient ftpClient(serverIP, controlSocket);

        try {
            ftpClient.login("admin", "admin");
            std::cout << "Welcome admin!" << std::endl;
            std::cout << "Enter 'help' to print all available commands" << std::endl;
        } catch (const LoginFailureException &e) {
            std::cerr << e.what() << std::endl;

            ftpClient.close();
            waitForEnter();
            return 0;
        }

        std::string userInput;

        while (true) {
            std::cout << ">>";
            std::getline(std::cin, userInput);

            if (userInput.empty()) {
                continue;
            } else if (userInput == "help") {
                FtpClient::displayCommands();
                continue;
            }

            std::istringstream iss(userInput);
            std::string commandStr;
            std::string argument;
            iss >> commandStr >> argument;

            FtpCommand command = stringToFtpCommand(commandStr);
            if (command == FtpCommand::EXIT) {
                break;
            }

            try {
                switch (command) {
                    case LS:
                        ftpClient.ls();
                        break;
                    case GET:
                        if (!argument.empty()) {
                            size_t dotPos = argument.find('.');
                            if (dotPos != std::string::npos && dotPos != 0 && dotPos < (argument.length() - 1)) {
                                ftpClient.get(argument);
                            } else {
                                std::cout
                                        << "Please specify a valid filename with filetype for the get command (e.g., file.txt)"
                                        << std::endl;
                            }
                        } else {
                            std::cout << "Please specify a filename for the get command" << std::endl;
                        }
                        break;
                    case ASCII:
                        ftpClient.setAsciiMode();
                        break;
                    case BINARY:
                        ftpClient.setBinaryMode();
                        break;
                    default:
                        std::cout << "Unknown command" << std::endl;
                        break;
                }
            } catch (const SocketConnectionFailureException &e) {
                std::cerr << e.what() << std::endl;
            } catch (const SocketDataFailureException &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        ftpClient.close();
    } catch (const SocketConnectionFailureException &e) {
        std::cerr << e.what() << std::endl;
        waitForEnter();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        waitForEnter();
    }

    return 0;
}