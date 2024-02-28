//
// Created by marco on 11/02/2024.
//

#include "includes/FtpClient.h"
#include "includes/enums/FtpCommand.h"
#include "includes/exeptions/LoginFailureException.h"

void waitForEnter() {
    std::cout << "Press Enter to continue..." << std::endl;
    std::string line;
    std::getline(std::cin, line);
}

int main() {
    if (FtpSocket::initWinSock() != 0) {
        std::cerr << "Failed to initialize WinSock" << std::endl;
        waitForEnter();
        return 0;
    }

    const std::string serverIP = "127.0.0.1";
    const uint16_t port = 21;

    try {
        FtpClient ftpClient(serverIP, port);

        try {
            ftpClient.login("admin", "admin");
            std::cout << "Welcome testuser!" << std::endl;
        } catch (const LoginFailureException &e) {
            std::cerr << e.what() << std::endl;

            ftpClient.close();
            waitForEnter();
            return 0;
        }

        std::string userInput;

        while (true) {
            FtpClient::displayCommands();

            std::cout << ">>";
            std::getline(std::cin, userInput);

            if (userInput.empty()) {
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
                                std::cout << "Please specify a valid filename with filetype for the get command (e.g., file.txt)" << std::endl;
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
            }

            waitForEnter();
        }

        ftpClient.close();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}