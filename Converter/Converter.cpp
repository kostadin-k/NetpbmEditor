#include "Converter.hpp"

void Converter::sameFromat(Pixel&, int) {
    return;
}
void Converter::fromP1toP2(Pixel& pixel, int max_value) {
    pixel.red = pixel.red * max_value;
    return;
}
void Converter::fromP1toP3(Pixel& pixel, int max_value) {
    int value = pixel.red * max_value;
    pixel.setRGB(value, value, value);
    return;
}
void Converter::fromP2toP1(Pixel& pixel, int max_value) {
    pixel.red = (pixel.red <= max_value / 2);
    return;
}
void Converter::fromP2toP3(Pixel& pixel, int) {
    int value = pixel.red;
    pixel.green = value;
    pixel.blue = value;
    return;
}
void Converter::fromP3toP1(Pixel& pixel, int max_value) {
    int value = pixel.grayscale();
    pixel.red = (value <= max_value / 2);
    return;
}
void Converter::fromP3toP2(Pixel& pixel, int) {
    int value = pixel.grayscale();
    pixel.red = value;
    return;
}

void (*Converter::converts[6]) (Pixel&, int) {
        fromP1toP2, fromP1toP3,
        fromP2toP1, fromP2toP3,
        fromP3toP1, fromP3toP2
};

Converter::Converter(ImageType from, ImageType to, int new_max) : new_max(new_max) {

    if (from != to) {
        int index = (from * (ImageType::LEN - 1) + to - (to > from));
        func = converts[index];
    }
    else {
        func = sameFromat;
    }
}

void Converter::apply(Pixel& pixel, int max_value) {
    func(pixel, max_value);

    if (new_max) {
        pixel.refactor(max_value, new_max);
    }
    
    return;
}