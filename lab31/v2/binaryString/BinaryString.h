#ifndef STRINGTEST_BINARYSTRING_H
#define STRINGTEST_BINARYSTRING_H

#include <string>
#include <cstring>
#include <cctype>

namespace ProxyServer {
    class BinaryString {
    public:
        BinaryString();

        BinaryString(const char *otherData, long long int otherLength);

        void setBinaryString(const char *otherData, long long int otherLength);

        void deleteData();

        BinaryString(BinaryString const &other);

        BinaryString subBinaryString(long long int positionStart, long long int positionEnd);

        friend BinaryString operator+(BinaryString &other1, BinaryString &other2);

        void add(BinaryString binaryString);

        BinaryString &operator=(const BinaryString &other);

        void clearData();

        void printer();

        ~BinaryString();

        long long int getLength() const;

        std::string toSting();

        char *getData();

        void copyData(const BinaryString& other);

        void copyAndCreateData(const BinaryString& other);

        void mallocNeedSize(long long int size);

        void addToMallocedBuf(const BinaryString& other);

        void deleteMallocedBuf();

        void setNewDataNotMalloc(const BinaryString& other, long long int start, long long int end);

        long long int getMallocedSize() const;

        void setNewDataNotMallocWithPtr(BinaryString *other, long long int start, long long int end);


        void shiftDataNotMalloc(const BinaryString& other, long long int size);

    private:
        long long int _dataSize = 0;
        char *_data = nullptr;

        long long int _mallocedSize = 0;

    };
}

#endif //STRINGTEST_BINARYSTRING_H
