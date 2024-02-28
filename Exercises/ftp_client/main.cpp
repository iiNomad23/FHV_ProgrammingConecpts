//
// Created by marco on 11/02/2024.
//

#include "includes/FtpClient.h"
#include "includes/enums/FtpCommand.h"

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
        } catch (const std::exception &e) {
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

            FtpCommand command = stringToFtpCommand(userInput);
            if (command == FtpCommand::EXIT) {
                break;
            }

            switch (command) {
                case LS:
                    ftpClient.ls();
                    break;
                case GET:
                    // TODO: GET command
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
        }

        ftpClient.close();
    } catch (const SocketConnectionFailureException& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}