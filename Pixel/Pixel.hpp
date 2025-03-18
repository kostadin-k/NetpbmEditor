#ifndef PIXEL_H
#define PIXEL_H
#include <string>
#include <exception>

#include "..\Helpers\Helpers.hpp"

/*
    class Pixel stores the valus of a pixel using 3 channels red, green and blue
*/

class Pixel {
private:
    int red{};
    int green{};
    int blue{};
    void copy(const Pixel& other); // copies the values of another pixel to the current one

public:
    Pixel() = default; // creates an empty pixel (0, 0, 0)
    Pixel(int r, int g, int b); // creates a pixel with red, green and blue values
    Pixel(const Pixel& other); // copy constructor
    Pixel& operator=(const Pixel& other); // copy operator
    
    void setRGB(int r, int g, int b); // sets values for red, green and blue 
    void setR(int r); // sets value for red 
    void setG(int g); // sets value for green 
    void setB(int b); // sets value for blue 

    int getR() const; // returns the value of the red channel
    int getG() const; // returns the value of the green channel
    int getB() const; // returns the value of the blue channel

    int grayscale() const; // returns grayscale value of the pixel using a formula;
    void refactor(int old_max, int new_max); // recalculates value of pixel for new max value 

    friend class Converter;
};

enum ImageType { //supported types of images
    P1, P2, P3, LEN
};

ImageType strToType(const std::string str);
std::string typeToStr(const ImageType ver);
ImageType extToType(const char* path);

#endif //PIXEL_H