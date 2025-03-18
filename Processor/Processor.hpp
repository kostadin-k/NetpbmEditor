#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include "Command/Command.hpp"
#include "../Editor/Editor.hpp"
#include <iostream>
#include <fstream>
/*
	class Processor is the command ui and executes functions at user unput
*/
class Processor {
private:
	Image* img;

	bool isValidCommand(Command& command); // checks if the command is valid based on a predefined switch case
	bool execute(Command& command); // executes action based on command, returns 0 if the command is exit in other cases returns 1

	static const char* list[]; // list with all the commands and their usage
	void printInfo(const char** info);

public:
	Processor(); // constructor 
	~Processor(); // destructor for list
	Processor(std::string path); // constructors a ui that opens a file

	Processor(const Processor&) = delete;
	Processor& operator=(const Processor&) = delete;

	void run(); //used as contunous input with while loop, stops when execute returns 0
};
#endif