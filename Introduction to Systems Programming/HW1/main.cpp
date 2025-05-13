#include "BlockChain.h"
#include "Utilities.h"
#include <iostream>


int main(int argc, char **argv) {

    if (argc != 4) {
        std::cerr << getErrorMessage() << "\n";
        return 0;
    }

    std::ofstream target;
    if ((string)argv[1] != "verify") {
        target.open(argv[3]);
    }
    std::ifstream source(argv[2]);
    BlockChain blockChain = BlockChainLoad(source);
    if ((string)argv[1] == "format") {
        BlockChainDump(blockChain, target);
    }

    if ((string)argv[1] == "hash") {
        BlockChainDumpHashed(blockChain, target);
    }

    if ((string)argv[1] == "compress") {
        BlockChainCompress(blockChain);
        BlockChainDump(blockChain, target);
    }

    if ((string)argv[1] == "verify") {
        std::ifstream targetVerify(argv[3]);
        if (BlockChainVerifyFile(blockChain, targetVerify)) {
            std::cout << "Verification passed" << std::endl;
        } else {
            std::cout << "Verification failed" << std::endl;
        }
    }
    BlockChainDeletion(&blockChain);


    return 0;
}