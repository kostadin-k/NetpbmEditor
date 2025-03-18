#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <sys/stat.h>

#include "ImageData.hpp"
#include "..\Pixel\Pixel.hpp"
#include "..\Converter\Converter.hpp"


class Image {
private:
    ImageType version;
    int max_value; // stores the max value of the channel(s)

    size_t width;
    size_t height;
    ImageData<Pixel>* data; // stores the pixels as 2D array
    
    std::string path; //path to the file
    bool isSaved;

    friend class Editor; // allows direct access to the editor
    
    void clearMeta(); // clears the metadata of the current image

    void readMeta(std::ifstream& ifstr); // reads the metadata of the current image
    void readData(std::ifstream& ifstr); // reads the data of the current image using other funtions in a switch case

    ImageData<Pixel>* readP1P2(std::ifstream& ifstr); // reads the data from a P1 and P2 file
    ImageData<Pixel>* readP3(std::ifstream& ifstr); // reads the data from a P3 file 
    
    void writeMeta(std::ofstream& ofstr); // writes the metadata of the current image
    void writeData(std::ofstream& ofstr, Converter& conv); // writes the data of the current image using other funtions in a switch case

    void writeP1(std::ofstream& ofstr, Converter& conv); // writes the data from a P1 file
    void writeP2(std::ofstream& ofstr, Converter& conv); // writes the data from a P2 file
    void writeP3(std::ofstream& ofstr, Converter& conv); // writes the data from a P3 file

    bool isOpen();

public:
    Image(); // constructs an image
    ~Image();
    Image(std::string path); // constructs an image from a file using open

    void newImage(size_t width, size_t height, std::string fill); // creates a new image with width and heightfilled with coloer fill
    void open(std::string path); // opens an image using readMeta and readData
    void close(); // closes image and promts if not saved

    void save(); // uses saveAs with an argument the path stored in the class to save the changes to the same file
    void saveAs(std::string path, bool overwrite = false); // saves an image to path using writeMeta and writeData

    void convert(ImageType type); // converts image to another type

    std::string metadata(); // converts image to another type
};

#endif //IMAGE_H
