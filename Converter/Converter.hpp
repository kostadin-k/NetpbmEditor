#ifndef CONVERTER_H
#define CONVERTER_H
#include "..\Pixel\Pixel.hpp"

class Converter {
private:
    static void sameFromat(Pixel&, int);
    static void fromP1toP2(Pixel&, int);
    static void fromP1toP3(Pixel&, int);
    static void fromP2toP1(Pixel&, int);
    static void fromP2toP3(Pixel&, int);
    static void fromP3toP1(Pixel&, int);
    static void fromP3toP2(Pixel&, int);

    static void (*converts[6]) (Pixel&, int); // list of all functions

    void (*func) (Pixel&, int); // currently selected function

    int new_max; // changes max value if specified 

public:
    Converter() = delete;
    Converter(const Converter&) = delete;
    Converter(ImageType from, ImageType to, int new_max = 0); // sets function to *func to be used based on ImageType
    void apply(Pixel& pixel, int max_value); // applies function in *func to Pixel
};

#endif //CONVERTER_H