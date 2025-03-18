#include "Editor.hpp"

ImageData<int>* Editor::diffuse_alg(int num, unsigned& factor, unsigned& offset) {
    ImageData<int>* res = nullptr;
    switch (num) {
    case 1: { // Floyd-Steinberg Dithering
        int matrix[6]{
            0, 0, 7,
            3, 5, 1
        };

        res = new ImageData<int>(2, 3, matrix);
        factor = 16;
        offset = 1;
        break;
    }
    case 2: { // False Floyd-Steinberg Dithering
        int matrix[4]{
            0, 3,
            3, 2
        };

        res = new ImageData<int>(2, 2, matrix);
        factor = 8;
        offset = 0;
        break;
    }
    case 3: { // Jarvis, Judice, and Ninke Dithering
        int matrix[15]{
            0, 0, 0, 7, 5,
            3, 5, 7, 5, 3,
            1, 3, 5, 3, 1
        };

        res = new ImageData<int>(3, 5, matrix);
        factor = 48;
        offset = 2;
        break;
    }
    case 4: { // Stucki Dithering
        int matrix[15]{
            0, 0, 0, 8, 4,
            2, 4, 8, 4, 2,
            1, 2, 4, 2, 1
        };

        res = new ImageData<int>(3, 5, matrix);
        factor = 42;
        offset = 2;
        break;
    }
    case 5: { // Atkinson Dithering
        int matrix[12]{
            0, 0, 1, 1,
            1, 1, 1, 0,
            0, 1, 0, 0
        };

        res = new ImageData<int>(3, 4, matrix);
        factor = 8;
        offset = 1;
        break;
    }
    case 6: { // Burkes Dithering
        int matrix[12]{
            0, 0, 0, 8, 4,
            2, 4, 8, 4, 2
        };
        res = new ImageData<int>(2, 5, matrix);
        factor = 32;
        offset = 2;
        break;

    }
    case 7: { // Sierra Dithering
        int matrix[15]{
            0, 0, 0, 5, 3,
            2, 4, 5, 4, 2,
            0, 2, 3, 2, 0
        };

        res = new ImageData<int>(3, 5, matrix);
        factor = 32;
        offset = 2;
        break;
    }
    case 8: { // Two-Row Sierra Dithering
        int matrix[12]{
            0, 0, 0, 4, 3,
            1, 2, 3, 2, 1
        };

        res = new ImageData<int>(2, 5, matrix);
        factor = 16;
        offset = 2;
        break;
    }
    case 9: { // Sierra Lite Dithering
        int matrix[6]{
            0, 0, 2,
            1, 1, 0
        };

        res = new ImageData<int>(2, 3, matrix);
        factor = 4;
        offset = 1;
        break;
    }
    default:
        throw std::invalid_argument("Invalid Aglorithm");
    }
    return res;
}

ImageData<int>* Editor::apply_alg(int num, unsigned& factor, unsigned& row_step, unsigned& col_step) {
    ImageData<int>* res = nullptr;
    switch (num) {
    case 10: { // Ordered dithering 4x4
        int matrix[16]{
             0,  8,  2, 10,
            12,  4, 14,  6,
             3, 11,  1,  9,
            15,  7, 13,  5
        };

        res = new ImageData<int>(4, 4, matrix);
        factor = 16;
        row_step = 4;
        col_step = 4;
        break;
    }
    case 11: { // Ordered dithering 8x8
        int matrix[64]{
             0, 32,  8, 40,  2, 34, 10, 42,
            48, 16, 56, 24, 50, 18, 58, 26,
            12, 44,  4, 36, 14, 46,  6, 38,
            60, 28, 52, 20, 62, 30, 54, 22,
             3, 35, 11, 43,  1, 55,  9, 41,
            51, 19, 59, 27, 49, 17, 57, 25,
            15, 47,  7, 39, 13, 45,  5, 37,
            63, 31, 55, 23, 61, 29, 53, 21
        };

        res = new ImageData<int>(8, 8, matrix);
        factor = 64;
        row_step = 8;
        col_step = 8;
        break;
    }
    default:
        throw std::invalid_argument("Invalid Aglorithm");
    }
    return res;
}

void Editor::diffuse(ImageData<Pixel>* data, size_t pixel_row, size_t pixel_col, int max_value, ImageData<int>* matrix, unsigned factor, unsigned offset) {
    size_t data_rows = data->getRows();
    size_t data_cols = data->getCols();

    size_t mat_rows = matrix->getRows();
    size_t mat_cols = matrix->getCols();

    bool is_max_red = data->at(pixel_row, pixel_col).getR() > max_value / 2;
    int new_red = max_value * is_max_red;
    int error_red = data->at(pixel_row, pixel_col).getR() - new_red;

    bool is_max_green = data->at(pixel_row, pixel_col).getG() > max_value / 2;
    int new_green = max_value * is_max_green;
    int error_green = data->at(pixel_row, pixel_col).getG() - new_green;

    bool is_max_blue = data->at(pixel_row, pixel_col).getB() > max_value / 2;
    int new_blue = max_value * is_max_blue;
    int error_blue = data->at(pixel_row, pixel_col).getB() - new_blue;

    data->at(pixel_row, pixel_col).setRGB(new_red, new_green, new_blue);

    size_t len = mat_rows * mat_cols;
    size_t i = 0;
    size_t j = 0;

    for (size_t k = offset + 1; k < len; k++) {
        i = k / mat_cols;
        j = k % mat_cols;

        if (pixel_col + j - (size_t)offset < data_cols && pixel_row + i < data_rows) {
            Pixel* pixel = &data->at(pixel_row + i, pixel_col - offset + j);
            int cell = matrix->at(i, j);

            pixel->setR(pixel->getR() + (int)((double)(cell * error_red) / (double)factor));
            pixel->setG(pixel->getG() + (int)((double)(cell * error_green) / (double)factor));
            pixel->setB(pixel->getB() + (int)((double)(cell * error_blue) / (double)factor));
        }
    }
}

void Editor::apply(ImageData<Pixel>* data, size_t pixel_row, size_t pixel_col, int max_value, ImageData<int>* matrix, unsigned factor) {
    size_t data_rows = data->getRows();
    size_t data_cols = data->getCols();

    size_t mat_rows = matrix->getRows();
    size_t mat_cols = matrix->getCols();

    size_t len = mat_rows * mat_cols;
    size_t i = 0;
    size_t j = 0;

    for (size_t k = 0; k < len; k++) {
        i = k / mat_cols;
        j = k % mat_cols;

        if (pixel_col + j < data_cols && pixel_row + i < data_rows) {
            Pixel* pixel = &data->at(pixel_row + i, pixel_col + j);
            int cell = matrix->at(i, j);

            int check = (double)(cell * max_value) / (double)factor;

            bool is_max_red = pixel->getR() > check;
            bool is_max_green = pixel->getG() > check;
            bool is_max_blue = pixel->getB() > check;

            pixel->setR(max_value * is_max_red);
            pixel->setG(max_value * is_max_green);
            pixel->setB(max_value * is_max_blue);
        }
    }
}

void Editor::dither_diffuse(Image& img, int algorithm) {
    unsigned diff = 0;
    unsigned offset = 0;

    ImageData<int>* matrix = nullptr;
    ImageData<Pixel>* buffer = nullptr;

    try {
        matrix = diffuse_alg(algorithm, diff, offset);
        buffer = new ImageData<Pixel>(*img.data);

        for (size_t i = 0; i < img.height; i++) {
            for (size_t j = 0; j < img.width; j++) {
                diffuse(buffer, i, j, img.max_value, matrix, diff, offset);
            }
        }

        delete matrix;
        delete img.data;
        img.data = buffer;
        img.isSaved = false;

    }
    catch (std::exception& e) {
        if (matrix) { delete matrix; }
        throw std::runtime_error(e.what());
    }
}

void Editor::dither_apply(Image& img, int algorithm) {
    unsigned diff = 0;
    unsigned row_step = 1;
    unsigned col_step = 1;

    ImageData<int>* matrix = nullptr;
    ImageData<Pixel>* buffer = nullptr;

    try {
        matrix = apply_alg(algorithm, diff, row_step, col_step);
        buffer = new ImageData<Pixel>(*img.data);

        for (size_t i = 0; i < img.height; i += row_step) {
            for (size_t j = 0; j < img.width; j += col_step) {
                apply(buffer, i, j, img.max_value, matrix, diff);
            }
        }

        delete matrix;
        delete img.data;
        img.data = buffer;
        img.isSaved = false;

    }
    catch (std::exception& e) {
        if (matrix) { delete matrix; }
        throw std::runtime_error(e.what());
    }
}

void Editor::crop(Image& img, size_t x1, size_t y1, size_t x2, size_t y2) {
    if (!img.isOpen()) {
        throw std::runtime_error("No image loaded!");
    }

    x1 = x1 > img.width ? img.width : x1;
    x1 = x2 > img.width ? img.width : x2;
    y1 = y1 > img.height ? img.height : y1;
    y2 = y2 > img.height ? img.height : y2;

    if (x1 < x2) swap(x1, x2);
    if (y1 < y2) swap(y1, y2);

    size_t new_width = x1 - x2 + 1;
    size_t new_height = y1 - y2 + 1;

    try {
        ImageData<Pixel>* buffer = new ImageData<Pixel>(new_height, new_width);

        for (size_t i = 0; i < new_height; i++) {
            for (size_t j = 0; j < new_width; j++) {
                buffer->at(i, j) = img.data->at(i + y2, j + x2);
            }
        }

        delete img.data;
        img.data = buffer;
        img.height = new_height;
        img.width = new_width;
        img.isSaved = false;

    }
    catch (std::exception& e) {
        throw std::runtime_error("Error while copying!");
    }
}

void Editor::resize(Image& img, size_t new_height, size_t new_width) {
    if (!img.isOpen()) {
        throw std::runtime_error("No image loaded!");
    }

    if (!new_height || !new_width) {
        throw std::invalid_argument("Size is too small!");
    }

    try {
        ImageData<Pixel>* buffer = new ImageData<Pixel>(new_height, new_width);

        for (size_t i = 0; i < new_height; i++) {
            for (size_t j = 0; j < new_width; j++) {
                size_t nn_Y = (int)round((double)i / (double)new_height * (double)img.height);
                size_t nn_X = (int)round((double)j / (double)new_width * (double)img.width);

                if (nn_Y >= img.height) nn_Y = img.height - 1;
                if (nn_X >= img.width) nn_X = img.width - 1;

                buffer->at(i, j) = img.data->at(nn_Y, nn_X);
            }
        }

        delete img.data;
        img.data = buffer;
        img.height = new_height;
        img.width = new_width;
        img.isSaved = false;

    }
    catch (std::exception& e) {
        throw std::runtime_error("Error while copying!");
    }
}

void Editor::resize(Image& img, double percentage) {
    size_t new_height = (size_t)((double)img.height * percentage / 100);
    size_t new_width = (size_t)((double)img.width * percentage / 100);
    resize(img, new_height, new_width);
}

void Editor::grayscale(Image& img) {
    if (!img.isOpen()) {
        throw std::runtime_error("No image loaded!");
    }

    if (img.version != ImageType::P3) return;

    try {
        ImageData<Pixel>* buffer = new ImageData<Pixel>(img.height, img.width);

        for (size_t i = 0; i < img.height; i++) {
            for (size_t j = 0; j < img.width; j++) {
                int value = img.data->at(i, j).grayscale();
                buffer->at(i, j).setRGB(value, value, value);
            }
        }

        delete img.data;
        img.data = buffer;
        img.isSaved = false;

    }
    catch (std::exception& e) {
        throw std::runtime_error("Error while copying!");
    }
}

void Editor::dither(Image& img, int algorithm) {
    if (!img.isOpen()) {
        throw std::runtime_error("No image loaded!");
    }

    if (algorithm >= 1 && algorithm <= 9) {
        dither_diffuse(img, algorithm);
    }
    else {
        dither_apply(img, algorithm);
    }
}