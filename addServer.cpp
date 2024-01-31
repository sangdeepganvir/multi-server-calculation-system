#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define int long long
#define nl '\n'

signed main() {
    int port = 9999;  // Port for additionServer
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating server socket" << endl;
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int opt = 1;
    int err = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    if (err < 0) {
        cerr << "Error setting socket options" << endl;
        close(serverSocket);
        return 1;
    }

    err = ::bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (err < 0) {
        cerr << "Error binding server socket" << endl;
        close(serverSocket);
        return 1;
    }

    err = listen(serverSocket, 5);
    if (err < 0) {
        cerr << "Error listening for connections" << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server listening on port " << port << endl;

    while (1) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            cerr << "Error accepting connection" << endl;
            continue;
        }

        char buffer[256];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        string res = buffer; 
        res = res.substr(0, bytesRead); 
        int ind = 0; 
        while(ind < bytesRead) {
            if (res[ind] == ' ') {
                break; 
            }
            ind++; 
        }
        int num1 = stoll(res.substr(0, ind)); 
        int num2 = stoll(res.substr(ind + 1, bytesRead - ind - 1));
        cout << num1 << " " << num2 << nl; 
        int result = num1 + num2;
        string resultStr = to_string(result);
        send(clientSocket, resultStr.c_str(), resultStr.size(), 0);
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}