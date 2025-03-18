#ifndef EDITOR_H
#define EDITOR_H
#include "../Image/Image.hpp"
#include <cmath>
/*
    Class Editor has funtions that manipulate an object of class Image
    Defined as a singleton
*/
class Editor {
private:
    Editor() {}; // editor defined as a singleton

    // selects an error diffusing dithering algorithm using switch case
    ImageData<int>* diffuse_alg(int num, unsigned& factor, unsigned& offset);
    // selects an ordering dithering algorithm using switch case
    ImageData<int>* apply_alg(int num, unsigned& factor, unsigned& row_step, unsigned& col_step);

    // used for diffusing the error from a selected pixel to the others around it using a matrix
    void diffuse(ImageData<Pixel>* data, size_t pixel_row, size_t pixel_col, int max_value, ImageData<int>* matrix, unsigned factor, unsigned offset);
    // used for applying a matrix of errors to a range of pixels
    void apply(ImageData<Pixel>* data, size_t pixel_row, size_t pixel_col, int max_value, ImageData<int>* matrix, unsigned factor);

    // both apply the respective algorythm to an image
    void dither_diffuse(Image& img, int algorithm);
    void dither_apply(Image& img, int algorithm);

public:
    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;
    static Editor& getInstance() {
        static Editor instance;
        return instance;
    }
    void crop(Image& img, size_t x1, size_t y1, size_t x2, size_t y2); // crops an image using the X and Y coordinates of two pixels
    void resize(Image& img, size_t new_height, size_t new_width);  // scales an image with new height and with using the Nearest neigbor algorithm
    void resize(Image& img, double percentage); // resizes an image with a percentage
    void grayscale(Image& img); // changes the image to grayscale using a formula
    void dither(Image& img, int algorithm); // selects a dithering algorithm using switch case
};

#endif