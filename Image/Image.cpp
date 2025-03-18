#include "Image.hpp"

Image::Image() {
    clearMeta();
    data = nullptr;
}

Image::Image(std::string path) {
    open(path);
}

Image::~Image() {
    if (data) delete data;
}

void Image::newImage(size_t width, size_t height, std::string fill) {
    if (width < 1 || height < 1) throw std::invalid_argument("Invalid size!");
    if (!isValidColor(fill)) throw std::invalid_argument("Invalid color!");

    ImageData<Pixel>* buffer = new ImageData<Pixel>(height, width);

    int r, g, b;
    hexToRGB(fill, r, g, b);

    if (!buffer) throw std::bad_alloc();
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            buffer->at(i,j).setRGB(r,g,b);
        }
    }
    
    if (data) {
        delete data;
    }
    data = buffer;

    this->width = width;
    this->height = height;

    version = P3;
    max_value = 255;

    path = "";
    isSaved = false;
}

void Image::readMeta(std::ifstream& ifstr) {
    ImageType version = strToType(getArg(ifstr));

    size_t width = strToSizeT(getArg(ifstr));
    size_t height = strToSizeT(getArg(ifstr));
    int max_value = 1;

    if (height == 0 || width == 0) throw ("Invalid size!");
    if (version != P1) {
        max_value = strToInt(getArg(ifstr));
        if (max_value == 0) throw ("Invalid max color!");
    }

    this->version = version;
    this->width = width;
    this->height = height;
    this->max_value = max_value;
}

ImageData<Pixel>* Image::readP1P2(std::ifstream& ifstr) {
    ImageData<Pixel>* buffer = new ImageData<Pixel>(height,width);

    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            int value = strToInt(getArg(ifstr));
            if(value > max_value) {
                delete buffer;
                throw std::invalid_argument("Invalid pixel");
            }
            buffer->at(i,j).setRGB(value, value, value);
            
        }
    }
    return buffer;
}

ImageData<Pixel>* Image::readP3(std::ifstream& ifstr) {
    ImageData<Pixel>* buffer = new ImageData<Pixel>(height, width);

    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            int r = strToInt(getArg(ifstr));
            int g = strToInt(getArg(ifstr));
            int b = strToInt(getArg(ifstr));

            if(r > max_value || g > max_value || b > max_value) {
                delete buffer;
                throw std::invalid_argument("Invalid pixel");
            }

            buffer->at(i, j).setRGB(r, g, b);
        }
    }
    return buffer;
}

void Image::writeP1(std::ofstream& ofstr, Converter& conv) {
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            conv.apply(data->at(i, j), max_value);

            ofstr << (bool)(data->at(i, j).getR()) << ' ';
        }
        ofstr << std::endl;
    }
}


void Image::writeP2(std::ofstream& ofstr, Converter& conv) {
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            conv.apply(data->at(i, j), max_value);

            ofstr << data->at(i,j).getR() << ' ';
        }
        ofstr << std::endl;
    }
}

void Image::writeP3(std::ofstream& ofstr, Converter& conv) {
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            conv.apply(data->at(i, j), max_value);

            ofstr << data->at(i, j).getR() << ' ';
            ofstr << data->at(i, j).getG() << ' ';
            ofstr << data->at(i, j).getB() << ' ';
        }
        ofstr << std::endl;
    }
}

bool Image::isOpen() {
    return width && height;
}

void Image::readData(std::ifstream& ifstr) {
    switch (version) {
        case P1:
        case P2: {
            data = readP1P2(ifstr);
            break;
        }
        
        case P3: {
            data = readP3(ifstr);
            break;
        }
        
        default: {
            break;
        }
    }
}

void Image::writeMeta(std::ofstream& ofstr) {
    ofstr << typeToStr(version) << std::endl;
    ofstr << width << ' ' << height << std::endl;

    if(version != P1) {
        ofstr << max_value << std::endl;
    }
}

void Image::writeData(std::ofstream& ofstr, Converter& conv) {
    switch (version) {
        case P1: {
            writeP1(ofstr, conv);
            break;
        }
        case P2: {
            writeP2(ofstr, conv);
            break;
        }
        case P3: {
            writeP3(ofstr, conv);
            break;
        }
        default:
            break;
    }
}

void Image::clearMeta() {
    version = P3;
    max_value = 0;

    width = 0;
    height = 0;

    path = "";
    isSaved = true;
}

void Image::open(std::string path) {
    std::ifstream file;

    file.open(path);
    if(!file) throw std::invalid_argument("Invalid path!");

    try {
        readMeta(file);
        readData(file);

    } catch (std::exception& e) {
        file.close();
        clearMeta();

        throw std::runtime_error("Couldn't read file!");
    }
    isSaved = true;
    this->path = path;
    file.close();
}

void Image::close() {
    if (!isSaved 
            && userConfirm("File not saved! Keep the changes?")) {
        
        try {
            std::string path;

            std::cout << "Enter file location: ";
            getline(std::cin, path);

            if (path == "") {
                std::cin.ignore();
            }
            else {
                saveAs(path);
            }
        } catch (std::exception& e) {
            throw std::runtime_error("File couldn't be closed!");
        }
            
    }
    delete data;
    data = nullptr;

    clearMeta();
}

void Image::saveAs(std::string path, bool overwrite) {
    if (!isOpen() || path.empty()) {
        throw std::runtime_error("No image loaded!");
    }

    ImageType to = extToType(path.c_str());

    if (!overwrite) {
        std::ifstream exists;
        exists.open(path);

        if (exists) {
            exists.close();
            if (!userConfirm("File exists! Overwrite?")) {
                throw std::invalid_argument("File not saved!");
            }
        }
    }

    Converter convert(version, to, max_value);
    version = to;

    std::ofstream file;
    file.open(path);

    if (!file) throw std::invalid_argument("Invalid path!");

    writeMeta(file);
    writeData(file, convert);

    file.close();

    isSaved = true;
    this->path = path;
}

void Image::save() {
    if (path.empty()) {
        throw std::invalid_argument("No previous file to update!");
    }
    saveAs(path,true);
}

void Image::convert(ImageType type) {
    if (!isOpen()) {
        throw std::runtime_error("No image loaded!");
    }

    if (type == version) return;

    Converter convertion(version, type);
    ImageData<Pixel>* buffer = nullptr;

    try {
        buffer = new ImageData<Pixel>(height, width);

        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                buffer->at(i, j) = data->at(i, j);
                convertion.apply(buffer->at(i, j), max_value);
            }
        }

        delete data;
        data = buffer;

        isSaved = false;
        version = type;
    }
    catch (std::exception& e) {
        if (buffer) { delete buffer; }
        throw std::runtime_error("Error while copying!");
    }
    return;
}

std::string Image::metadata() {
    if (!isOpen()) {
        throw std::runtime_error("No image loaded!");
    }

    std::string meta;
    meta = meta + "Type: " + typeToStr(version) + ", ";
    meta = meta + "Max color value: ";

    if (version == ImageType::P1) {
        meta += "1\n";
    }
    else {
        meta = meta + intToStr(max_value) + '\n';
    }

    meta = meta + "Width: " + intToStr(width) + ", ";
    meta = meta + "Height: " + intToStr(height) + '\n';

    meta = meta + "Path: ";

    if (path.empty()) {
        meta += "(unsaved)";
    }
    else {
        meta = meta + path;
        if (!isSaved) {
            meta += '*';
        }
    }

    return meta;
}