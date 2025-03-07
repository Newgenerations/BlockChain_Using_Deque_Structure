#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <string>
#include <openssl/sha.h> // SHA-256 Hashing
#include <thread>
#include <chrono>

using namespace std;

// 🔑 Utility function: Generate SHA-256 hash
string sha256(const string &input)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)input.c_str(), input.length(), hash);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << (int)hash[i];
    }
    return ss.str();
}

// 📦 Block structure
class Block
{
public:
    int index;
    string previousHash;
    string timestamp;
    string data;
    string hash;
    int nonce;

    Block(int idx, string prevHash, string txData)
    {
        index = idx;
        previousHash = prevHash;
        data = txData;
        nonce = 0;
        timestamp = to_string(time(0));
        hash = calculateHash();
    }

    // 🔄 Calculate block hash
    string calculateHash()
    {
        string blockData = to_string(index) + previousHash + timestamp + data + to_string(nonce);
        return sha256(blockData);
    }

    // ⛏️ Proof-of-Work: Find valid hash
    void mineBlock(int difficulty)
    {
        string target(difficulty, '0');
        while (hash.substr(0, difficulty) != target)
        {
            nonce++;
            hash = calculateHash();
        }
        cout << "✅ Block " << index << " mined: " << hash << endl;
    }
};

// ⛓️ Blockchain
class Blockchain
{
private:
    vector<Block> chain;
    int difficulty;

public:
    Blockchain(int diff)
    {
        difficulty = diff;
        chain.push_back(createGenesisBlock());
    }

    Block createGenesisBlock()
    {
        return Block(0, "0", "Genesis Block");
    }

    Block getLatestBlock()
    {
        return chain.back();
    }

    void addBlock(Block newBlock)
    {
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    void displayBlockchain()
    {
        cout << "\n⛓️ Blockchain:\n";
        for (Block b : chain)
        {
            cout << "🔗 Block " << b.index << " [Prev: " << b.previousHash.substr(0, 8) << "] => Hash: " << b.hash.substr(0, 8) << endl;
        }
    }
};

// 🌍 P2P Node (Simulating a Network Peer)
class Node
{
private:
    Blockchain blockchain;
    int nodeID;

public:
    Node(int id, int difficulty) : blockchain(difficulty)
    {
        nodeID = id;
    }

    void mineBlock(string data)
    {
        cout << "\n⛏️ Node " << nodeID << " is mining a new block...\n";
        Block newBlock(blockchain.getLatestBlock().index + 1, blockchain.getLatestBlock().hash, data);
        blockchain.addBlock(newBlock);
    }

    void syncBlockchain(Node &otherNode)
    {
        cout << "🔄 Node " << nodeID << " syncing with Node " << otherNode.nodeID << "...\n";
        if (blockchain.getLatestBlock().index < otherNode.blockchain.getLatestBlock().index)
        {
            blockchain = otherNode.blockchain;
            cout << "✅ Node " << nodeID << " updated to the latest blockchain!\n";
        }
    }

    void showBlockchain()
    {
        blockchain.displayBlockchain();
    }
};

#endif // BLOCKCHAIN_H
