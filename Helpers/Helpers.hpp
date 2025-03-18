#ifndef HELPERS_H
#define HELPERS_H
#include <string>
#include <fstream>
#include <exception>
#include <cstring>
#include <iostream>

bool isValidColor(std::string color); // checks if a string color is a valid HEX color
void hexToRGB(std::string color, int &r, int &g, int &b); // converts a HEX color to RGB

std::string getFileName(const std::string& path); // returns a char pointer with the name of the file
std::string getFileExtention(const std::string& path); // returns a char pointer with the extention of the file

bool isPercentage(std::string str); // checks if a string str is a valid percentage
size_t indexPointPrc(std::string str); // returns the index of a floating dot
double strToPrc(std::string str);  // converts a string to a percetnage (double)
std::string intToStr(int num);

bool isNumber(std::string str); // checks if a string str is a valid number
int strToInt(const std::string str); // converts a string to a number (int)
size_t strToSizeT(const std::string str); // converts a string to a number (size_t)

void swap(size_t &a, size_t &b); // swaps two variables

bool isEqualString(std::string str_1, std::string str_2); // checks if the string is equal despite the case

bool hasComment(std::string query); // checks if a string query has a comment in it
void removeComment(std::string& query); // removes the comment if it finds one
std::string getArg(std::ifstream& ifstr); // returns the next argument from a file

bool userConfirm(const std::string& info);

#endif