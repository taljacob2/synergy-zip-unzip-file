

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <fstream>
#include <vector>

class Serializer {

  private:
    int sizeOfField = 0;

  private:
    int stringLength = 0;

    // private:
    //   /**
    //    * Used for `vector.size()`
    //    */
    //   long unsigned int vectorSize = 0;

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

    // TODO: remove.
    // public:
    //   template<typename T>
    //   void serializeVectorOfSerializables(std::ofstream &file, std::vector<T>
    //           &vector) {
    //       static_assert(std::is_base_of<Serializable, T>::value,
    //                     "T must derive from Serializable");
    //
    //       vectorSize = sizeof(vector.size());
    //       file.write(reinterpret_cast<const char *>(&vectorSize),
    //                  sizeof(vectorSize));
    //
    //       for (int i = 0; i < vectorSize ; ++i) {
    //           (reinterpret_cast<Serializable>(vector[i]))
    //       }
    //       file.write(reinterpret_cast<const char *>(&vector), vectorSize);
    //   }
    //
    // public:
    //   template<typename T> T deserializeVectorOfSerializables(std::ifstream
    //                                             &file) {
    //       T returnValue;
    //       file.read(reinterpret_cast<char *>(&sizeOfField), sizeof(sizeOfField));
    //       file.read(reinterpret_cast<char *>(&returnValue), sizeOfField);
    //       return returnValue;
    //   }
};

#endif // SERIALIZER_H
