#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include "Blockchain.h"

using namespace std;

// P2P Node class
class Node
{
private:
    string ip;
    int port;
    int sock;
    struct sockaddr_in serverAddr, clientAddr;
    Blockchain blockchain;
    vector<string> peerIps;

public:
    Node(string ipAddr, int portNum, int difficulty)
        : blockchain(difficulty)
    {
        ip = ipAddr;
        port = portNum;
    }

    void initServer()
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            cerr << "Socket creation failed!" << endl;
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            cerr << "Bind failed!" << endl;
            exit(1);
        }

        listen(sock, 5);
        cout << "Server started on port " << port << endl;
    }

    void acceptClient()
    {
        socklen_t clientAddrSize = sizeof(clientAddr);
        int newSock = accept(sock, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (newSock < 0)
        {
            cerr << "Client acceptance failed!" << endl;
            return;
        }

        cout << "Connected to peer: " << inet_ntoa(clientAddr.sin_addr) << endl;
        thread receiveThread(&Node::receiveData, this, newSock);
        receiveThread.detach();
    }

    void receiveData(int clientSock)
    {
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = recv(clientSock, buffer, sizeof(buffer), 0)) > 0)
        {
            buffer[bytesRead] = '\0';
            string receivedData(buffer);
            cout << "Received from peer: " << receivedData << endl;
            // Here we could implement more logic to update the blockchain
        }
    }

    void connectToPeer(string peerIp)
    {
        int peerSock = socket(AF_INET, SOCK_STREAM, 0);
        if (peerSock < 0)
        {
            cerr << "Socket creation failed!" << endl;
            return;
        }

        struct sockaddr_in peerAddr;
        peerAddr.sin_family = AF_INET;
        peerAddr.sin_port = htons(port);
        peerAddr.sin_addr.s_addr = inet_addr(peerIp.c_str());

        if (connect(peerSock, (struct sockaddr *)&peerAddr, sizeof(peerAddr)) < 0)
        {
            cerr << "Connection to peer failed!" << endl;
            return;
        }

        cout << "Connected to peer: " << peerIp << endl;
        string message = "Hello from node at " + ip;
        send(peerSock, message.c_str(), message.length(), 0);
    }

    void broadcast(string message)
    {
        for (const string &peerIp : peerIps)
        {
            connectToPeer(peerIp);
        }
    }

    void addPeer(string peerIp)
    {
        peerIps.push_back(peerIp);
    }

    void mineAndAddBlock(string data)
    {
        Block newBlock(blockchain.getLatestBlock().index + 1, blockchain.getLatestBlock().hash, data);
        blockchain.addBlock(newBlock);
        broadcast("New block mined and added to the blockchain!");
    }

    void showBlockchain()
    {
        blockchain.displayBlockchain();
    }

    void start()
    {
        initServer();

        while (true)
        {
            acceptClient();
            // Simulate block mining and peer connection
            mineAndAddBlock("Transaction data " + to_string(rand() % 1000));
            this_thread::sleep_for(chrono::seconds(5));
        }
    }
};

#endif // NODE_H
