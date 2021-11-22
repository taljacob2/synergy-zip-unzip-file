

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <fstream>
#include <iostream>
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
    std::string *
    convertBinaryStringToBinaryBitsAndMoveToTheLeft(std::string &str) {
        auto          stringOfBits = new std::string();
        unsigned char bitToInsert;
        int           i = 0;
        for (; i < str.length(); ++i) {
            if ((i % 8) == 0) {
                stringOfBits->push_back('\0'); // Insert empty char.
            }
            bitToInsert = str[i] - '0';
            bitToInsert <<= (8 - ((i + 1) % 8)); // Move to the left.
            ((*stringOfBits)[stringOfBits->length() - 1]) |= bitToInsert;
        }

        return stringOfBits;
    }

  public:
    void writeBinaryStringToBinaryFile(std::ofstream &ofstream,
                                       std::string &  binaryStringOfAllFile) {
        std::string *stringOfBits =
                this->convertBinaryStringToBinaryBitsAndMoveToTheLeft(
                        binaryStringOfAllFile);
        for (unsigned char byteToInsert : *stringOfBits) {
            ofstream.write(reinterpret_cast<const char *>(&byteToInsert),
                           sizeof(byteToInsert));
        }

        delete stringOfBits;
    }

  public:
    std::string *convertBinaryBitsToBinaryString(char charToConvert) {
        auto str = new std::string();
        for (int i = 0; i < 8; ++i) {
            unsigned char extractedChar = charToConvert;
            extractedChar >>= 7; // Move to the right.
            extractedChar += '0';
            str->push_back(extractedChar);
            charToConvert <<= 1;
        }
        return str;
    }

  public:
    std::string &
    readBinaryBitsToBinaryString(std::ifstream &ifstream, char &seenCharacter,
                                 std::string &binaryStringOfAllFile) {
        while (true) {
            ifstream.read(reinterpret_cast<char *>(&seenCharacter),
                          sizeof(seenCharacter));
            if (ifstream.eof()) { break; }

            std::string *seenBinaryString =
                    this->convertBinaryBitsToBinaryString(seenCharacter);
            binaryStringOfAllFile += *(seenBinaryString);

            delete seenBinaryString;
        }
        return binaryStringOfAllFile;
    }
};

#endif // SERIALIZER_H
