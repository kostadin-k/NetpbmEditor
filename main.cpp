#include <iostream>
#include "Processor/Processor.hpp"
using namespace std;

int main(int argc, char *argv[]) {
	try {
        if (argc >= 2) {
            Processor UI(argv[1]);
            UI.run();
        }
        else {
            Processor UI;
            UI.run();
        }
        return 0;
    }
    catch (std::exception &e) {
        cerr << e.what();
        return 1;
    }
}