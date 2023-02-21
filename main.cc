#include "watopoly.h"
#include <iostream>
void usage() {
    std::cout << "watopoly [-load] [file] [-testing]" << std::endl;
}
int main(int argc, char** argv) {
    std::string firstArg;
    bool testing = false;
    std::string file;
    if (argc == 2) {
        firstArg.assign(argv[1]);
        if (firstArg == "-testing") {
            testing = true;
        } else {
            usage();
            return 1;
        }
    } else if (argc == 3) {
        firstArg.assign(argv[1]);
        if (firstArg == "-load") {
            file.assign(argv[2]);
        } else {
            usage();
            return 1;
        }
    } else if (argc == 4) {
        firstArg.assign(argv[1]);
        std::string thirdArg;
        thirdArg.assign(argv[3]);
        if (firstArg == "-load" && thirdArg == "-testing") {
            file.assign(argv[2]);
            testing = true;
        } else {
            usage();
            return 1;
        }
    } else if (argc != 1) {
        usage();
        return 1;
    }
    Watopoly watopoly{testing, file};
    watopoly.play();
}
