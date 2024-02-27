
#include "includes/FTPClient.h"

void waitForEnter() {
    std::cout << "Press Enter to continue..." << std::endl;
    std::string line;
    std::getline(std::cin, line);
}

int main() {
    if (MySocket::initWinSock() != 0) {
        std::cerr << "failed to initialize WinSock" << std::endl;
        waitForEnter();
        return 0;
    }

    const std::string serverIP = "127.0.0.1";
    const uint16_t port = 21;

    FTPClient ftpClient(serverIP, port);
    if (!ftpClient.isConnected()) {
        waitForEnter();
        return 0;
    }

    uint16_t responseCode;

    responseCode = ftpClient.login("testuser", "mypass");
    if (responseCode == 0) {
        std::cerr << "This no good :(" << std::endl;
        waitForEnter();
        return 0;
    }

    std::cout << "Welcome testuser!" << std::endl;

    // TODO list commands and user input


    waitForEnter();
    return 0;
}