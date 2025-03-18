#include "Helpers.hpp"  

bool isValidColor(std::string color){
    if (color.size() != 7) {
        return false;
    }

    if (color[0] != '#') return false;

    for (int i = 1; i < 7; i++) {
        if ((color[i] < '0' || color[i] > '9') &&
            (toupper(color[i]) < 'A' || toupper(color[i]) > 'F')) {
            return false;
        }
    }

    return true;
}

void hexToRGB(std::string color, int &r, int &g, int &b) {
    r = 16;
    g = 16;
    b = 16;

    r *= color[1] <= '9' ? color[1] - '0' : toupper(color[1]) - 'A' + 10;
    r += color[2] <= '9' ? color[2] - '0' : toupper(color[2]) - 'A' + 10;

    g *= color[3] <= '9' ? color[3] - '0' : toupper(color[3]) - 'A' + 10;
    g += color[4] <= '9' ? color[4] - '0' : toupper(color[4]) - 'A' + 10;

    b *= color[5] <= '9' ? color[5] - '0' : toupper(color[5]) - 'A' + 10;
    b += color[6] <= '9' ? color[6] - '0' : toupper(color[6]) - 'A' + 10;
}

std::string getFileName(const std::string& path) {
    const char* buffer = strrchr(path.c_str(), '/');

    if (buffer == nullptr) {
        buffer = strrchr(path.c_str(), '\\');
        if (buffer == nullptr) {
            buffer = path.c_str();
        } else ++buffer;
    } else ++buffer;

    std::string str(buffer);
    return str;
}

std::string getFileExtention(const std::string& path) {
    const char* buffer = strrchr(path.c_str(), '.');

    std::string str(buffer);
    return str;
}

bool isPercentage(std::string str) {
    if (str.size() < 2) return false;
    bool point = false;
    for(size_t i = 0; i < str.size()-1; i++) {
        if (str[i] < '0' || str[i] > '9') {
            if (str[i] == '.' && !point) {
                point = true;
                continue;
            }
            return false;
        }
        //if((str[i] < '0' || str[i] > '9') && str[i] != '.') return false;
    }
    if (str[str.size() - 1] != '%' || (str.size() - 2 && str[str.size() - 2] == '.')) return false;
    return true;
}

size_t indexPointPrc(std::string str) {
    for(size_t i = 0; i < str.size()-1; i++) {
        if(str[i] == '.') return i;
    }
    return str.size() - 1;
}

double strToPrc(std::string str) {
    if(!isPercentage(str)) throw std::invalid_argument("Not a percentage!");
    double buffer = 0.0;
    size_t point_index = indexPointPrc(str);
    size_t test = str.size() - 1 - point_index;

    for(size_t i = 0; i < point_index; i++) {
        buffer *= 10;
        buffer += (double)(str[i] - '0');
    }

    double post_point = 0.0;
    size_t post_point_count = 0;
    /*size_t post_point_count = str.size() - 1 - point_index - 1;*/
    for(size_t i = point_index + 1; i < str.size()-1; i++) {
        post_point *= 10;
        post_point += (double)(str[i] - '0');
        post_point_count++;
    }

    for (size_t i = 0; i < post_point_count; i++) {
        post_point /= 10;
    }

    return buffer + post_point;
}

bool isNumber(std::string str) {
    for(size_t i = 0; i < str.size(); i++) {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

int strToInt(const std::string str) {
    if (!isNumber(str)) throw std::invalid_argument("Not a number!");   
    int buffer = 0;
    for(size_t i = 0; i < str.size(); i++) {
        buffer *= 10;
        buffer += (str[i] - '0');
    }
    return buffer;
}

size_t strToSizeT(const std::string str) {
    if (!isNumber(str)) throw std::invalid_argument("Not a number!");   
    size_t buffer = 0;
    for(size_t i = 0; i < str.size(); i++) {
        buffer *= 10;
        buffer += (str[i] - '0');
    }
    return buffer;
}


std::string intToStr(int num) {
    std::string res;
    while (num) {
        char digit = (num % 10 + '0');
        res = digit + res;
        num /= 10;
    }
    if (res.empty()) {
        return "0";
    }
    return res;
}

void swap(size_t &a, size_t &b) {
    size_t temp;
    temp = a;
    a = b;
    b = temp;
}

bool isEqualString(std::string str_1, std::string str_2) {
    if (str_1 == str_2) return true;
    if (str_1.size() != str_2.size()) return false;
    size_t len = str_1.size();
    const char* ch_1 = str_1.c_str();
    const char* ch_2 = str_2.c_str();
    for (size_t i = 0; i < len; i++) {
        if (std::toupper(ch_1[i]) != std::toupper(ch_2[i])) return false;
    } 
    return true;
}

bool hasComment(std::string query) {
    for(size_t i = 0; i < query.size(); i++) {
        if(query[i] == '#') return true;
    }
    return false;
}

void removeComment(std::string& query) {
    size_t i = 0;
    for(i; i < query.size(); i++) {
        if(query[i] == '#') break;
    }
    query.erase(query.begin() + i, query.end());
}

std::string getArg(std::ifstream& ifstr) {
    std::string buffer;
    std::string temp;
    ifstr>>buffer;
    if(!ifstr) return "";
    while(ifstr && buffer[0] == '#') {
        getline(ifstr,temp);
        ifstr >> buffer;
    }

    if(hasComment(buffer)) {
        removeComment(buffer);
        getline(ifstr,temp);
    }
    return buffer;
}

bool userConfirm(const std::string& info) {
    std::string resp;
    do {
        std::cout << info << " [Y/N]: ";
        getline(std::cin, resp);
        bool test1 = resp.empty();
        bool test2 = resp.length() == 1;
        bool test3 = toupper(resp[0]) == 'Y';
        bool test4 = toupper(resp[0]) == 'N';
    } while (!(resp.empty() || resp.length() == 1 &&
        (toupper(resp[0]) == 'Y' || toupper(resp[0]) == 'N')));

    return toupper(resp[0]) != 'N';
}
