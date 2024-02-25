
#include "FTPClient.h"

int main() {
    if (MySocket::initWinSock() != 0) {
        std::cerr << "failed to initialize WinSock" << std::endl;
        return 0;
    }

    const std::string serverIP = "127.0.0.1";
    const int port = 21;

    FTPClient ftpClient(serverIP, port);
    if (!ftpClient.isConnected()) {
        return 0;
    }

    ftpClient.login("testuser", "mypass");

    // TODO list commands and user input


    return 0;
}