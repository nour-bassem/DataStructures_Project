// Server.cpp

#include <iostream>
#include <string>
#include <WS2tcpip.h> 
#include <thread>
#include <ctime>
//#include <vector>
#include "LinkedList.h"
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
using namespace std;

struct ClientInfo {
    SOCKET socket;
    string name;
};

class Message {
public:
    string content;
    string timestamp;

    Message(string content, string timestamp) : content(content), timestamp(timestamp) {}
};

LinkedList<string> chatHistory; // Store chat history

//void sendChatHistory(SOCKET socket) {
//    for (const auto& message : chatHistory) {
//        send(socket, message.c_str(), message.size() + 1, 0);
//    }
//}

void sendChatHistory(SOCKET socket, const LinkedList<string>& chatHistory) {
    typename LinkedList<string>::Node* current = chatHistory.getFirst();
    while (current != nullptr) {
        const std::string& message = current->data;
        send(socket, message.c_str(), message.size() + 1, 0);
        current = current->next;
    }
}


//void clientHandler(ClientInfo client, LinkedList<ClientInfo>& clients) {
//    char buf[4096];
//    char timeBuffer[80];
//
//    sendChatHistory(client.socket); // Send previous chat history to the new client
//
//    while (true) {
//        ZeroMemory(buf, 4096);
//        int bytesReceived = recv(client.socket, buf, 4096, 0);
//        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
//            cerr << "Client disconnected: " << client.name << endl;
//            // Remove the disconnected client from the vector
//            for (auto it = clients.begin(); it != clients.end(); ++it) {
//                if (it->socket == client.socket) {
//                    clients.erase(it);
//                    break;
//                }
//            }
//            break;
//        }
//
//        // Get current timestamp
//        time_t rawTime;
//        struct tm timeInfo;
//        time(&rawTime);
//        localtime_s(&timeInfo, &rawTime);
//        strftime(timeBuffer, sizeof(timeBuffer), "[%I:%M %p] ", &timeInfo);
//
//        // Construct the message with sender's name and timestamp
//        string message = client.name + ": " + string(buf, 0, bytesReceived) + " " + timeBuffer;
//
//        // Add the message to chat history
//        chatHistory.push_back(message);
//
//        // Forward the message to all other connected clients
//        for (const auto& otherClient : clients) {
//            if (otherClient.socket != client.socket) {
//                send(otherClient.socket, message.c_str(), message.size() + 1, 0);
//            }
//        }
//    }
//}

void clientHandler(ClientInfo client, LinkedList<ClientInfo>& clients, LinkedList<std::string>& chatHistory) {
    char buf[4096];
    char timeBuffer[80];

    // Send previous chat history to the new client
    sendChatHistory(client.socket, chatHistory);

    typename LinkedList<ClientInfo>::NodePtr current; // Declare 'current' at a higher scope

    while (true) {
        ZeroMemory(buf, 4096);
        int bytesReceived = recv(client.socket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            cerr << "Client disconnected: " << client.name << endl;

            // Remove the disconnected client from the LinkedList
            current = clients.getFirst();
            int index = 0;
            while (current != nullptr) {
                if (current->data.socket == client.socket) {
                    clients.erase(index);
                    break;
                }
                current = current->next;
                ++index;
            }

            break;
        }

        // Get current timestamp
        time_t rawTime;
        struct tm timeInfo;
        time(&rawTime);
        localtime_s(&timeInfo, &rawTime);
        strftime(timeBuffer, sizeof(timeBuffer), "[%I:%M %p] ", &timeInfo);

        // Construct the message with sender's name and timestamp
        string message = client.name + ": " + string(buf, 0, bytesReceived) + " " + timeBuffer;

        // Add the message to chat history
        chatHistory.push_back(message);

        // Forward the message to all other connected clients
        current = clients.getFirst(); // Reuse 'current' for this loop
        while (current != nullptr) {
            if (current->data.socket != client.socket) {
                send(current->data.socket, message.c_str(), message.size() + 1, 0);
            }
            current = current->next;
        }
    }
}




int main() {
    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return -1;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return -1;
    }

    // Bind the socket to an IP address and port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton...

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    LinkedList<ClientInfo> clients;

    while (true) {
        SOCKET clientSocket = accept(listening, nullptr, nullptr);

        // Get user's name
        char nameBuf[1024];
        int bytesReceived = recv(clientSocket, nameBuf, 1024, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            cerr << "Error in receiving name from client" << endl;
            continue;
        }
        string userName = string(nameBuf, 0, bytesReceived);

        cout << "New client connected: " << userName << endl;

        // Create a ClientInfo object and add it to the vector of clients
        ClientInfo newClient;
        newClient.socket = clientSocket;
        newClient.name = userName;
        clients.push_back(newClient);
        
        // Create a thread to handle this client
        thread clientThread(clientHandler, newClient, ref(clients), ref(chatHistory));
        clientThread.detach(); // Detach the thread to run independently
    }

    // Close listening socket (this code is unreachable in this example)
    closesocket(listening);

    // Cleanup Winsock
    WSACleanup();

}