#include "Processor.hpp"

void Processor::printInfo(const char** info) {
    const char** iter = info;
    while (*iter) {
        std::cout << *iter << '\n';
        ++iter;
    }
}

const char* Processor::list[] = {
    "Command list:",
    "new <height> <width> <color (#hex)>",
    "open <path>",
    "close",
    "save [path]",
    "info",
    "crop <point1-x> <point1-y> <point2-x> <point2-y>",
    "resize <percentage (decimal%)> OR resize <height> <width>",
    "dither <algorithm (number)>",
    "exit",
    "Dither algorithms:",
    "1. Floyd-Steinberg Dithering",
    "2. Fake Floyd-Steinberg Dithering",
    "3. Jarvis, Judice, and Ninke Dithering",
    "4. Stucki Dithering",
    "5. Atkinson Dithering",
    "6. Burkes Dithering",
    "7. Sierra Dithering (5x3)",
    "8. Sierra Dithering (5x2)",
    "9. Sierra Dithering (3x2)",
    "10. Ordered dithering (4x4)",
    "11. Ordered dithering (8x8)"
};

Processor::Processor() {
    img = new Image;
    std::cout << "Netpbm Editor\n";
    std::cout << "Supported formats: P1, P2, P3\n";
    std::cout << "Type \"help\" for the list of commands\n";
};

Processor::~Processor() {
    delete img;
};

Processor::Processor(std::string path) {
    img = new Image;
    img->open(path);
}

void Processor::run() {
    std::string input;
    bool active = true;
    do {
        std::cout << std::endl;
        std::getline(std::cin, input);
        if (input.empty()) {
            continue;
        }

        Command command(input);
        if(!isValidCommand(command)) {
            continue;
        }
        active = execute(command);

    } while (active);
}

bool Processor::isValidCommand(Command& command) {
    if (isEqualString(command[0], "exit")) {
        if (command.size() < 1) {
            std::cerr << "Usage: "<< list[9] << "\n";
            return false;
        }
        return true;
        
    } else if (isEqualString(command[0], "new")) {
        if (command.size() < 4) {
            std::cerr << "Usage: "<< list[1] << "\n";
            return false;
        }
        return true;

    } else if (isEqualString(command[0], "open")) {
        if (command.size() < 2) {
            std::cerr << "Usage: "<< list[2] << "\n";
            return false;
        }
        return true;

    } else if (isEqualString(command[0], "close")) {
        if (command.size() < 1) {
            std::cerr << "Usage: "<< list[3] << "\n";
            return false;            
        }
        return true;

    } else if (isEqualString(command[0], "save")) {
        if (command.size() < 1) {
            std::cerr << "Usage: "<< list[4] << "\n";
            return false;            
        }
        return true;

    } else if (isEqualString(command[0], "info")) {
        if (command.size() < 1) {
            std::cerr << "Usage: "<< list[5] << "\n";
            return false;            
        }
        return true;
        
    } else if (isEqualString(command[0], "crop")) {
        if (command.size() < 5) {
            std::cerr << "Usage: "<< list[6] << "\n";
            return false;            
        }
        return true;

    } else if (isEqualString(command[0], "resize")) {
        if (command.size() < 2) {
            std::cerr << "Usage: "<< list[7] << "\n";
            return false;            
        }
        return true;

    } else if (isEqualString(command[0], "dither")) {
        if (command.size() < 2) {
            std::cerr << "Usage: "<< list[8] << "\n";
            return false;            
        }
        return true;

    } else if (isEqualString(command[0], "help")) {
        if (command.size() < 1) {
            std::cerr << "Usage: help\n";
            return false;            
        }
        return true;

    } else {
        std::cerr << "Unknown command!\n";
        return false;
    }
}

bool Processor::execute(Command& command) {
    if (isEqualString(command[0], "exit")) {
        if (img)
        {
            img->close();
        }
        std::cout << "Exiting program...";
        return 0;
    }

    if (isEqualString(command[0], "new")) {
        try {
            img->newImage(strToSizeT(command[1]),strToSizeT(command[2]),command[3]);
            std::cout << "Created new document with size " << command[1] << 'x' << command[2] << " and filled with " << command[3]  << "\n";
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }

    } else if (isEqualString(command[0], "open")) {
        try {
            img->open(command[1]);
            std::cout << "Successfully opened " << getFileName(command[1])  << "\n";
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }

    } else if (isEqualString(command[0], "close")) {
        try {
            img->close();
            std::cout << "Successfully closed file\n";
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }

    } else if (isEqualString(command[0], "save")) {
        try {
            if (command.size() < 2) {
                img->save();
                std::cout << "Successfully saved file\n";
            } else {
                img->saveAs(command[1]);
                std::cout << "Successfully saved file as " << getFileName(command[1]) << "\n";
            }

        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }

    } else if (isEqualString(command[0], "info")) {
        try {
            std::cout << img->metadata() << '\n';
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }
        
    } else if (isEqualString(command[0], "crop")) {
        try {
            Editor::getInstance().crop(*img, strToSizeT(command[1]), strToSizeT(command[2]),
                                        strToSizeT(command[3]), strToSizeT(command[4]));
            std::cout << "Successfully cropped image from " << command[1] << ',' << command[2]<< " to " << command[3]  << ',' << command[4]  << "\n";
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }
        

    } else if (isEqualString(command[0], "resize")) {
        if (command.size() == 2) {
            try {
                Editor::getInstance().resize(*img, strToPrc(command[1]));
                std::cout << "Successfully resized image by " << command[1]  << "\n";
            } catch (std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        } else {
            try {
                Editor::getInstance().resize(*img, strToSizeT(command[1]), strToSizeT(command[2]));
                std::cout << "Successfully resized image to " << command[1] << 'x' << command[2]  << "\n";
            } catch (std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        }

    } else if (isEqualString(command[0], "dither")) {
        try {
            Editor::getInstance().dither(*img, strToInt(command[1]));
            std::cout << "Successfully dittered image using alorithm #" << list[strToInt(command[1]) + 10]  << "\n";
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    } else if (isEqualString(command[0], "help")) {
        printInfo(list);
    } else {
        std::cerr << "Invalid input!\n";
    }

    return 1;
}