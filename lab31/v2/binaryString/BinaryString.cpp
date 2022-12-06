#include <iostream>
#include "BinaryString.h"

using namespace ProxyServer;

BinaryString::BinaryString() {
    _data = nullptr;
    _dataSize = 0;
    _mallocedSize = 0;
}

BinaryString::BinaryString(const char *otherData, long long int otherLength) {
    _dataSize = otherLength;
    if (otherLength > 0) {
        _data = new char[otherLength];
        _mallocedSize = otherLength;
        for (long long int i = 0; i < otherLength; i++) {
            _data[i] = otherData[i];
        }
    } else {
        _data = nullptr;
    }
}

long long int BinaryString::getLength() const {
    return _dataSize;
}

std::string BinaryString::toSting() {
    if (_dataSize == 0 || _dataSize == -1) {
        return std::string("");
    } else {
        std::string newString;
        for (long long int i = 0; i < _dataSize; i++) {
            newString += _data[i];
        }
        return newString;
    }
//    return std::string(_data);
}

char *BinaryString::getData() {
    return _data;
}

BinaryString::~BinaryString() {
    delete[] _data;
}

BinaryString::BinaryString(const BinaryString &other) {
    _dataSize = other._dataSize;
    _data = new char[other._dataSize];
    _mallocedSize = other._dataSize;
    for (long long int i = 0; i < other._dataSize; i++) {
        _data[i] = other._data[i];
    }
}

BinaryString BinaryString::subBinaryString(long long int positionStart, long long int positionEnd) {
    BinaryString binaryString;
    if (_dataSize >= positionEnd - positionStart) {
        binaryString._dataSize = positionEnd - positionStart;
    } else {
        binaryString._dataSize = _dataSize;
    }

    binaryString._data = new char[binaryString._dataSize];
    _mallocedSize = binaryString._dataSize;

    for (long long int i = 0; i < binaryString._dataSize; i++) {
        binaryString._data[i] = _data[i + positionStart];
    }
    return binaryString;
}

BinaryString operator+(BinaryString &other1, BinaryString &other2) {
    long long int size = other1.getLength() + other2.getLength();
    char *buf = new char[size];
    for (long long int i = 0; i < other1.getLength(); i++) {
        buf[i] = other1.getData()[i];
    }
    for (long long int i = 0; i < other2.getLength(); i++) {
        buf[i + other1.getLength()] = other2.getData()[i];
    }
    BinaryString binaryString(buf, size);
    delete[] buf;
    return binaryString;
}


void BinaryString::add(BinaryString binaryString) {
    long long int newSize = _dataSize + binaryString._dataSize;
    char *newData = new char[newSize];
    for (long long int i = 0; i < _dataSize; i++) {
        newData[i] = _data[i];
    }
    for (long long int i = 0; i < binaryString.getLength(); i++) {
        newData[i + _dataSize] = binaryString.getData()[i];
    }
    delete[] _data;

    _data = newData;
    _dataSize = newSize;
    _mallocedSize = newSize;
}

BinaryString &BinaryString::operator=(const BinaryString &other) {
    delete[] _data;
    _dataSize = other._dataSize;
    _data = new char[_dataSize];
    _mallocedSize = _dataSize;
    for (long long int i = 0; i < other._dataSize; i++) {
        _data[i] = other._data[i];
    }
    return *this;
}

void BinaryString::printer() {
    for (long long int i = 0; i < _dataSize; i++) {
        std::cout << _data[i];
    }
    std::cout << std::endl;
}

void BinaryString::clearData() {
    for (long long int i = 0; i < _mallocedSize; i++)
        _data[i] = 0;
    _dataSize = 0;
}

void BinaryString::setBinaryString(const char *otherData, long long int otherLength) {
    _dataSize = otherLength;
    for (long long int i = 0; i < otherLength; i++) {
        _data[i] = otherData[i];
    }
}

void BinaryString::copyData(const BinaryString &other) {
    _dataSize = other._dataSize;
    for (long long int i = 0; i < other._dataSize; i++) {
        _data[i] = other._data[i];
    }
}

void BinaryString::setNewDataNotMalloc(const BinaryString &other, long long int start, long long int end) {
    if (_mallocedSize < end - start) {
        delete[] _data;
        _data = new char[end - start + 10];
        _mallocedSize = end - start + 10;
    }
    _dataSize = end - start;
    for (long long int i = start; i < end; i++) {
        _data[i - start] = other._data[i];
    }
}


void BinaryString::shiftDataNotMalloc(const BinaryString &other, long long int size) {
    _dataSize = _dataSize - size;
    for (long long int i = size; i < _dataSize; i++) {
        _data[i - size] = other._data[i];
    }
}


void BinaryString::setNewDataNotMallocWithPtr(BinaryString *other, long long int start, long long int end) {
    _dataSize = end - start;
    for (long long int i = start; i < end; i++) {
        _data[i - start] = other->_data[i];
    }
}

void BinaryString::copyAndCreateData(const BinaryString &other) {
    _dataSize = other._dataSize;
    _data = new char[_dataSize];
    _mallocedSize = _dataSize;
    for (long long int i = 0; i < other._dataSize; i++) {
        _data[i] = other._data[i];
    }
}


void BinaryString::mallocNeedSize(long long int size) {
    _data = new char[size + 10];
    _mallocedSize = size + 10;
    _dataSize = 0;
}

void BinaryString::addToMallocedBuf(const BinaryString &other) {
    if (_mallocedSize < _dataSize + other._dataSize) {
        char *newBuf = new char[_dataSize + other._dataSize + 10];

        _mallocedSize = _dataSize + other._dataSize + 10;
        for (int i = 0; i < _dataSize; i++) {
            newBuf[i] = _data[i];
        }
        delete[] _data;

        for (long long int i = _dataSize; i < _dataSize + other._dataSize; i++) {
            newBuf[i] = other._data[i - _dataSize];
        }
        _dataSize = _dataSize + other._dataSize;
        _data = newBuf;

    } else {
        for (int i = 0; i < other._dataSize; i++) {
            _data[i + _dataSize] = other._data[i];
        }
        _dataSize = _dataSize + other._dataSize;
    }
}

void BinaryString::deleteMallocedBuf() {
    delete[] _data;
    _mallocedSize = 0;
    _dataSize = 0;
}

void BinaryString::deleteData() {
    delete[] _data;
    _mallocedSize = 0;
    _data = nullptr;
    _dataSize = 0;
}

long long int BinaryString::getMallocedSize() const {
    return _mallocedSize;
}


