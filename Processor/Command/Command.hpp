#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>
#include <exception>
#include <cassert>
/*
	class Commmand processes a string and into a command by splitting the arguments
*/
class Command  {
public:
	Command() = delete; // disabled default constrictor as the string can't be modified elsewhere
	Command(const std::string& command); // constructor with string input, saves the command the string command and uses splitCommand 

	std::string& operator[](const size_t index); // resturns argumrnt at index

	bool empty() const; // checks if the command is an empty string
	size_t size() const; // returns the number of arguments the command has
	
	const char* getString() const; // returns the string as cstring

private:
	std::string command;  
	std::vector<std::string> args;
	void splitCommand(); //splits command at any interval and igonres intervals if the argument is in quotes

};
#endif