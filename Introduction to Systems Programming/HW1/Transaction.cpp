#include <string>
#include <fstream>
#include "Transaction.h"
#include "Utilities.h"

using std::string;
using std::ofstream;


void TransactionDumpInfo(const Transaction &transaction, ofstream &file) {
    file << "Sender Name:" << transaction.sender << "\n";
    file << "Receiver Name:" << transaction.receiver << "\n";
    file << "Transaction Value:" << transaction.value << "\n";
}


/**
 * TransactionHashedMessage - Return the hash key of the given transaction.
 *                          - it uses the hash function.
 *
 * @param transaction - the transaction we want to hash.
 * @return
 */
string TransactionHashedMessage(const Transaction &transaction) {
    return (hash(transaction.value, transaction.sender, transaction.receiver));
}


bool TransactionVerifyHashedMessage(
        const Transaction &transaction,
        string hashedMessage
) {
    string temp = hash(transaction.value, transaction.sender,
                       transaction.receiver);
    if (temp == hashedMessage) {
        return true;
    } else {
        return false;
    }
}
