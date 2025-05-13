#include <string>
#include <fstream>
#include <iostream>
#include "Transaction.h"
#include "BlockChain.h"
#include "Utilities.h"

using std::cout;

int BlockChainGetSize(const BlockChain &blockChain) {
    const BlockChain *temp = &blockChain;
    unsigned int counter = 0;
    while (temp != nullptr) {
        counter++;
        temp = temp->previous;
    }
    return counter;
}


int
BlockChainPersonalBalance(const BlockChain &blockChain, const string &name) {
    const BlockChain *temp = &blockChain;
    int sum = 0;
    while (temp != nullptr) {
        if (temp->transaction.receiver == name) {
            sum += temp->transaction.value;
        }
        if (temp->transaction.sender == name) {
            sum -= temp->transaction.value;
        }
        temp = temp->previous;
    }
    return sum;
}


void BlockChainAppendTransaction(
        BlockChain &blockChain,
        unsigned int value,
        const string &sender,
        const string &receiver,
        const string &timestamp
) {
    BlockChain *newblock = new BlockChain();
    *newblock = blockChain;
    blockChain.previous = newblock;
    blockChain.transaction.value = value;
    blockChain.transaction.sender = sender;
    blockChain.transaction.receiver = receiver;
    blockChain.timestamp = timestamp;


}


void BlockChainAppendTransaction(
        BlockChain &blockChain,
        const Transaction &transaction,
        const string &timestamp
) {
    BlockChain *newblock = new BlockChain();
    *newblock = blockChain;
    blockChain.transaction = transaction;
    blockChain.timestamp = timestamp;
    blockChain.previous = newblock;

}


BlockChain BlockChainLoad(ifstream &file) {
    BlockChain firstBlock;
    BlockChain *ptr = &firstBlock;
    bool firstCheck = true;

    firstBlock.timestamp = "0";

    while (!file.eof()) {
        if (firstCheck) {
            firstCheck = false;
            file >> firstBlock.transaction.sender;
            file >> firstBlock.transaction.receiver;
            file >> firstBlock.transaction.value;
            file >> firstBlock.timestamp;
            firstBlock.previous = nullptr;
            continue;
        }
        BlockChain *newBlock = new BlockChain();
        file >> newBlock->transaction.sender;
        file >> newBlock->transaction.receiver;
        file >> newBlock->transaction.value;
        file >> newBlock->timestamp;
        newBlock->previous = nullptr;
        ptr->previous = newBlock;
        ptr = newBlock;
    }
    return firstBlock;
}


void BlockChainDump(const BlockChain &blockChain, ofstream &file) {
    file << "BlockChain Info:" << "\n";
    int counter = 1;
    const BlockChain *current = &blockChain;
    while (current != nullptr) {
        file << counter << "." << "\n";
        file << "Sender Name: " << current->transaction.sender << "\n";
        file << "Receiver Name: " << current->transaction.receiver << "\n";
        file << "Transaction Value: " << current->transaction.value << "\n";
        file << "Transaction timestamp: " << current->timestamp << "\n";
        counter++;
        current = current->previous;
    }
}


void BlockChainDumpHashed(const BlockChain &blockChain, ofstream &file) {
    const BlockChain *helper = &blockChain;
    bool first = true;
    while (helper != nullptr) {
        if (!(first)) {
            file << "\n";
        }
        file << hash(helper->transaction.value,
                     helper->transaction.sender,
                     helper->transaction.receiver);
        helper = helper->previous;
        first = false;
    }
}


bool BlockChainVerifyFile(const BlockChain &blockChain, std::ifstream &file) {
    if ((blockChain.timestamp == "0")) {
        if (file.peek() == std::ifstream::traits_type::eof()) {
            return true;
        } else {
            return false;
        }
    }
    const BlockChain *helper = &blockChain;
    string token;
    while (helper != nullptr) {
        file >> token;
        string currentHash = hash(helper->transaction.value,
                                  helper->transaction.sender,
                                  helper->transaction.receiver);
        if (token != currentHash) {
            return false;
        }
        helper = helper->previous;
    }
    if (!(file.eof())) {
        return false;
    }
    return true;
}


void BlockChainCompress(BlockChain &blockChain) {
    BlockChain *ptr = &blockChain;
    BlockChain *helper;
    while (ptr->previous != nullptr) {
        if (ptr->transaction.sender == ptr->previous->transaction.sender &&
            ptr->transaction.receiver == ptr->previous->transaction.receiver) {
            helper = ptr->previous->previous;
            ptr->transaction.value += ptr->previous->transaction.value;
            delete ptr->previous;
            ptr->previous = helper;
        } else {
            ptr = ptr->previous;
        }
    }
}


void BlockChainTransform(BlockChain &blockChain, updateFunction function) {
    BlockChain *ptr = &blockChain;
    while (ptr != nullptr) {
        ptr->transaction.value = function(ptr->transaction.value);
        ptr = ptr->previous;
    }
}


void BlockChainDeletion(BlockChain *blockChain) {
    blockChain = blockChain->previous;
    while (blockChain != nullptr) {
        BlockChain *temp = blockChain;
        blockChain = blockChain->previous;
        delete temp;
    }
}