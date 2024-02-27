
#include "includes/FtpClient.h"
#include "includes/enums/FtpCommand.h"

void waitForEnter() {
    std::cout << "Press Enter to continue..." << std::endl;
    std::string line;
    std::getline(std::cin, line);
}

int main() {
    if (FtpSocket::initWinSock() != 0) {
        std::cerr << "Failed to initialize WinSock :(" << std::endl;
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
        ftpClient.login("testuser", "mypass");

        std::cout << "Welcome testuser!" << std::endl;
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
                    // Handle LS command
                    break;
                case GET:
                    // Handle GET command
                    break;
                case ASCII:
                    // Handle ASCII command
                    break;
                case BINARY:
                    // Handle BINARY command
                    break;
                default:
                    std::cout << "Unknown command" << std::endl;
                    break;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        waitForEnter();
        return 0;
    }

    return 0;
}