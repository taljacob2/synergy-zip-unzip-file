

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <fstream>
#include <vector>

class Serializer {

  private:
    int sizeOfField = 0;

  private:
    int stringLength = 0;

  public:
    template<typename T>
    void serializePrimitiveField(std::ofstream &file, T field) {
        T fieldCopy = field;
        sizeOfField = sizeof(fieldCopy);
        file.write(reinterpret_cast<const char *>(&sizeOfField),
                   sizeof(sizeOfField));
        file.write(reinterpret_cast<const char *>(&fieldCopy), sizeOfField);
    }

  public:
    template<typename T> T deserializePrimitiveField(std::ifstream &file) {
        T returnValue;
        file.read(reinterpret_cast<char *>(&sizeOfField), sizeof(sizeOfField));
        file.read(reinterpret_cast<char *>(&returnValue), sizeOfField);
        return returnValue;
    }

  public:
    template<typename T>
    void serializeStringField(std::ofstream &file, std::string stringField) {
        stringLength = stringField.length();
        file.write(reinterpret_cast<const char *>(&stringLength),
                   sizeof(stringLength));
        file.write(reinterpret_cast<const char *>(stringField.data()),
                   stringLength);
    }

  public:
    template<typename T> T deserializeStringField(std::ifstream &file) {
        std::string returnValue;
        file.read(reinterpret_cast<char *>(&stringLength),
                  sizeof(stringLength));
        returnValue.resize(stringLength);
        file.read(reinterpret_cast<char *>((void *) returnValue.data()),
                  stringLength);
        return returnValue;
    }

  public:
    std::vector<unsigned char> *
    convertBinaryStringToBinaryBitsAndMoveToTheLeft(std::string &str) {
        auto          vector = new std::vector<unsigned char>();
        unsigned char bitToInsert;
        int           i = 0;
        for (; i < str.length(); ++i) {
            if ((i % 8) == 0) {
                vector->push_back('\0'); // Insert empty char.
            }
            bitToInsert = str[i] - '0';
            (*vector)[vector->size() - 1] |= bitToInsert;
            (*vector)[vector->size() - 1] <<= 1;
        }

        // Move to the left the last byte.
        (*vector)[vector->size() - 1] <<= (8 - (i % 8));
        return vector;
    }

  public:
    void writeBinaryStringToBinaryFile(std::ofstream &ofstream,
                                       std::string &  binaryStringOfAllFile) {
        std::vector<unsigned char> *vector =
                this->convertBinaryStringToBinaryBitsAndMoveToTheLeft(
                        binaryStringOfAllFile);
        for (unsigned char byteToInsert : *vector) {
            ofstream.write(reinterpret_cast<const char *>(&byteToInsert),
                           sizeof(byteToInsert));
        }

        delete vector;
    }
};

#endif // SERIALIZER_H
