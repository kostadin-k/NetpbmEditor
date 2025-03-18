#include "Pixel.hpp"

void Pixel::copy(const Pixel& other) {
    this->red = other.red;
    this->green = other.green;
    this->blue = other.blue;
}

Pixel::Pixel(int r, int g, int b) 
    : red(r), green(g), blue(b) {}

Pixel::Pixel(const Pixel& other) {
    copy(other); 
}

Pixel& Pixel::operator=(const Pixel& other) {
    copy(other); 
    return *this; 
}

void Pixel::setRGB(int r, int g, int b) { 
    red = r; 
    green = g; 
    blue = b; 
}
void Pixel::setR(int r) {
    red = r; 
}

void Pixel::setG(int g) {
    green = g; 
}

void Pixel::setB(int b) {
    blue = b; 
}

int Pixel::getR() const {
    return red; 
}

int Pixel::getG() const {
    return green; 
}

int Pixel::getB() const {
    return blue; 
}

int Pixel::grayscale() const {
    return red * 0.21 + green * 0.72 + blue * 0.07; 
}

void Pixel::refactor(int old_max, int new_max) {
    red = (red * new_max) / old_max;
    green = (green * new_max) / old_max;
    blue = (blue * new_max) / old_max;
}

ImageType strToType(const std::string str) {
    if (str == "P1") {
        return P1;
    }
    else if (str == "P2") {
        return P2;
    }
    else if (str == "P3") {
        return P3;
    }
    else {
        throw std::invalid_argument("Unsupported version!");
    }
}

std::string typeToStr(const ImageType ver) {
    switch (ver) {
        case P1: {
            return "P1";
        }
        case P2: {
            return "P2";
        }
        case P3: {
            return "P3";
        }
        default: {
            throw std::runtime_error("Unsupported version!");
        }
    }
}

ImageType extToType(const char* path) {
    std::string ext = getFileExtention(path);

    if (isEqualString(ext, ".pbm")) {
        return ImageType::P1;

    }
    else if (isEqualString(ext, ".pgm")) {
        return ImageType::P2;

    }
    else if (isEqualString(ext, ".ppm")) {
        return ImageType::P3;
    }

    throw std::runtime_error("Undeclared version!");
}