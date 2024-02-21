// Client.cpp

#include <iostream>
#include <string>
#include <WS2tcpip.h> // Remember to link against 'Ws2_32.lib'
#include <thread>
#include <ctime>
//#include <unordered_map>
#include "HashTable.h"

#pragma comment (lib, "Ws2_32.lib")
using namespace std;

class Message {
public:
    string content;
    string timestamp;

    Message(string content, string timestamp) : content(content), timestamp(timestamp) {}
};

void receiveMessages(SOCKET sock) {
    char buf[4096];
    char timeBuffer[80];
    while (true) {
        ZeroMemory(buf, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            cerr << "Server disconnected" << endl;
            break;
        }

        // Display received message
        cout << string(buf, 0, bytesReceived) << endl;
    }
}

int main() {
    // Get user's name
    //unordered_map<string, string> loginInfo = { {"david", "1234"}, {"fady", "2345"}, {"joe", "3456"} , {"nour", "1206"},{"amir","4321"}};

    HashTable loginInfo;
    loginInfo.insert("david", "1234");
    loginInfo.insert("fady", "2345");
    loginInfo.insert("joe", "3456");
    loginInfo.insert("nour", "4567");
    loginInfo.insert("amir", "5678");

    // Get user's login credentials
    string userName, password;
    cout << "Enter your name: ";
    cin >> userName;
    cout << "Enter your password: ";
    cin >> password;

    // Validate login credentials
    if (loginInfo.find(userName) == nullptr) {
        cerr << "Invalid login credentials. Exiting..." << endl;
        return -1;
    }

    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return -1;
    }

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return -1;
    }

    // Connect to the server
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to the server! Quitting" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    thread receiveThread(receiveMessages, sock);
    receiveThread.detach(); // Detach the thread to run independently

    // Continuously send messages to the server
    string userInput;
    while (true) {
        getline(cin, userInput);

        if (userInput.size() > 0) {
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult == SOCKET_ERROR) {
                cerr << "Error in sending message. Quitting" << endl;
                break;
            }
        }
    }

    // Close socket and cleanup
    closesocket(sock);
    WSACleanup();

}