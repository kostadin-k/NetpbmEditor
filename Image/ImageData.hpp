#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include <stdexcept>
#include <iostream>
#include <exception>


/*
	A template class that creates and manages a 2D array
	Used in Image to handle the pixels
*/

template <class T>
class ImageData {
private:
	T** arr;
	size_t rows;
	size_t cols;
	void releaseToIndex(const size_t index); // releases all the memory up to an index
	T** allocate(const size_t rows, const size_t cols); // allocates a 2D array dynamically as a block
	void copy(const ImageData<T>& other); //deletes the contents if this (if there are any) allocates a new 2D array, copies the contesnts of other 
	void create(const size_t rows, const size_t cols);

public:
	ImageData() = delete; // default constructor disabled to avoid creation of an empty array
	~ImageData(); // deletes array using releaseToIndex
	
	ImageData(size_t rows, size_t cols); // contructs an array with with rows and columns size
	ImageData(const ImageData<T>& other); // copy constrictor using copy function
	ImageData(size_t rows, size_t cols, const T* arr);
	ImageData<T>& operator=(const ImageData<T>& other); // copy operator using copy function
	T& at(size_t i, size_t j); // returns the element at posion i and j
	
	size_t getRows() const; // returns the number of rows in the array
	size_t getCols() const;	// returns the number of columns in the array

};

template <class T>
void ImageData<T>::releaseToIndex(const size_t index) {
	for (size_t i = 0; i < index; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}

template <class T>
T** ImageData<T>::allocate(const size_t rows, const size_t cols) {
	T** buffer = new T*[rows];
	if (!buffer) throw std::bad_alloc();
	for(size_t i = 0; i < rows; i++) {
		buffer[i] = new T[cols];
		if(!buffer[i]) {
			releaseToIndex(i);
			throw std::bad_alloc();
		}
	}
	return buffer;
}

template <class T>
void ImageData<T>::copy(const ImageData<T>& other) {
	if(cols > 0 || rows > 0) {
		releaseToIndex(rows);
	}
	T** buffer = allocate(other.rows, other.cols);
	for(size_t i = 0; i < other.rows; i++) {
		for(size_t j = 0; j < other.cols; j++) {
			buffer[i][j] = other.arr[i][j];
		}
	}
	arr = buffer;
	cols = other.cols;
	rows = other.rows;
}

template<class T>
void ImageData<T>::create(const size_t rows, const size_t cols) {
	arr = allocate(rows, cols);
	this->rows = rows;
	this->cols = cols;
}

template <class T>
ImageData<T>::ImageData(size_t rows, size_t cols) {
	create(rows, cols);
}

template <class T>
ImageData<T>::ImageData(const ImageData<T>& other) {
	copy(other);
}

template<class T>
ImageData<T>::ImageData(size_t rows, size_t cols, const T* data) {
	create(rows, cols);
	size_t len = rows * cols;
	for (size_t i = 0; i < len; i++) {
		arr[i/cols][i%cols] = data[i];
	}
}

template <class T>
ImageData<T>::~ImageData() {
	releaseToIndex(rows);
}

template <class T>
ImageData<T>& ImageData<T>::operator=(const ImageData<T>& other) {
	copy(other);
	return *this;
}

template <class T>
T& ImageData<T>::at(size_t i, size_t j) {
	if (i >= rows || j >= cols) throw std::invalid_argument("Invalid position");
	return arr[i][j];
}

template <class T>
size_t ImageData<T>::getRows() const {
	return rows;
}

template <class T>
size_t ImageData<T>::getCols() const {
	return cols;
}

#endif