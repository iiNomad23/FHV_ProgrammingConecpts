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

    FtpClient ftpClient(serverIP, port);
    if (!ftpClient.isConnected()) {
        waitForEnter();
        return 0;
    }

    try {
        ftpClient.login("admin", "admin");
        std::cout << "Welcome testuser!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
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
            // TODO: close
            break;
        }

        switch (command) {
            case LS:
                // TODO: LS command
                break;
            case GET:
                // TODO: GET command
                break;
            case ASCII:
                // TODO: ASCII command
                break;
            case BINARY:
                // TODO: BINARY command
                break;
            default:
                std::cout << "Unknown command" << std::endl;
                break;
        }
    }

    return 0;
}