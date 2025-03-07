#include "header/Blockchain.h"
#include "header/Node.h"

int main()
{
    srand(time(0));

    // Create two nodes (Simulating P2P network)
    Node node1(1, 4);
    Node node2(2, 4);

    // Simulation loop (5 mining cycles)
    for (int i = 1; i <= 5; i++)
    {
        cout << "\n=====================================";
        cout << "\n🔄 Simulation Cycle #" << i;
        cout << "\n=====================================\n";

        // Randomly choose a node to mine
        int miner = rand() % 2 + 1;
        string transactionData = "Tx " + to_string(i) + ": Alice -> Bob $" + to_string(rand() % 100);

        if (miner == 1)
        {
            node1.mineBlock(transactionData);
        }
        else
        {
            node2.mineBlock(transactionData);
        }

        // Nodes sync to maintain longest chain
        node1.syncBlockchain(node2);
        node2.syncBlockchain(node1);

        // Show blockchain state
        node1.showBlockchain();

        // 🕒 Simulate delay
        this_thread::sleep_for(chrono::seconds(2));
    }

    cout << "\n📌 Final Blockchain State:";
    node1.showBlockchain();

    return 0;
}

// #include <iostream>
// #include <thread>
// #include <vector>
// #include "header/Node.h"

// using namespace std;

// int main()
// {
//     vector<thread> nodeThreads;

//     // Simulate multiple nodes
//     Node node1("127.0.0.1", 8081, 4);
//     Node node2("127.0.0.1", 8082, 4);
//     Node node3("127.0.0.1", 8083, 4);

//     // Start node servers in separate threads
//     nodeThreads.push_back(thread(&Node::start, &node1));
//     nodeThreads.push_back(thread(&Node::start, &node2));
//     nodeThreads.push_back(thread(&Node::start, &node3));

//     // Add peers to each node
//     node1.addPeer("127.0.0.1");
//     node2.addPeer("127.0.0.1");
//     node3.addPeer("127.0.0.1");

//     // Join threads to simulate continuous running of nodes
//     for (auto &t : nodeThreads)
//     {
//         t.join();
//     }

//     return 0;
// }
