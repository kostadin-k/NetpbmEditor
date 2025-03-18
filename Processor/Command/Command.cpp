#include "Command.hpp"  

void Command::splitCommand() {
    std::string buffer;
    bool quoted = false;

    for (std::size_t i = 0; i < command.size(); i++) {
        if (isspace((unsigned char)command[i]) && !quoted) {
            if (!isspace((unsigned char)command[i + 1])) {
                args.push_back(buffer);
                buffer.clear();
            }  
        } else if(command[i] == '"') {
            quoted = !quoted;
            if (!quoted) {
                args.push_back(buffer);
                buffer.clear();
            }
        } else buffer.push_back(command[i]);
    }

    if(!buffer.empty()) args.push_back(buffer);
}

Command::Command(const std::string& command)
{
	this->command = command;
    splitCommand();
}

std::string& Command::operator[](const size_t index) {
    return args[index];
}

size_t Command::size() const {
    return args.size();
}

bool Command::empty() const {
    return args.empty();
}

const char* Command::getString() const {
    return command.c_str();
}