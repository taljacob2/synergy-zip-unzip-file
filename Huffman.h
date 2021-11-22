//
// Created by Tal on 21-Nov-21.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "Entry.h"
#include "MinHeapWrapper.h"
#include "VectorExtension.h"
#include <fstream>
#include <iostream>

class Huffman {

  public:
    class Table : Serializable {

      public:
        /**
         * @brief A char that is placed at the end of a binary-file which had
         *        been zipped with a huffman-table - to mark that the table has ended.
         */
        static char constexpr END_OF_FILE = '\0';

      private:
        /**
         * @brief Contains a *table* whereas for each row, the *key* is a
         *        *character*, and the *value* is the *huffman-code* for that
         *        character.
         */
        std::vector<Entry<char, std::string> *> vector;

      public:
        std::vector<Entry<char, std::string> *> &getVector() { return vector; }

      public:
        Table() = default;

      public:
        virtual ~Table() {
            VectorExtension<Entry<char, std::string> *>::deleteVectorOfPointers(
                    vector);
        }

      public:
        friend std::ostream &operator<<(std::ostream &os, const Table &table) {
            auto tableVector = table.vector;
            os << "[";
            if (!tableVector.empty()) { os << *tableVector[0]; }
            for (long unsigned int i = 1; i < tableVector.size(); ++i) {
                os << ", " << *tableVector[i];
            }
            os << "]";
            return os;
        }

      public:
        void serialize(std::ofstream &file) override {
            long unsigned int vectorSize = vector.size();
            file.write(reinterpret_cast<const char *>(&vectorSize),
                       sizeof(vectorSize));

            for (long unsigned int i = 0; i < vectorSize; ++i) {
                vector[i]->serializeKeyFieldValueString(file);
            }
        }

      public:
        void deserialize(std::ifstream &file) override {
            long unsigned int vectorSize;
            file.read(reinterpret_cast<char *>(&vectorSize),
                      sizeof(vectorSize));
            vector.reserve(vectorSize * sizeof(Entry<char, std::string> *));

            for (long unsigned int i = 0; i < vectorSize; ++i) {
                auto *entry = new Entry<char, std::string>();
                entry->deserializeKeyFieldValueString(file);
                vector.push_back(entry);
            }
        }

      public:
        int findIndexByKey(char keyToFind) {
            return VectorExtension<Entry<char, std::string>>::
                    findIndexByPredicate<char>(vector, keyToFind,
                                               predicateOfKeyInCharStringEntry);
        }

      public:
        Entry<char, std::string> *findByKey(char keyToFind) {
            int index = findIndexByKey(keyToFind);
            if (index ==
                VectorExtension<Entry<char, std::string>>::INDEX_NOT_FOUND) {
                return nullptr;
            }

            return vector[index];
        }

      public:
        int findIndexByValue(std::string &valueToFind) {
            return VectorExtension<Entry<char, std::string>>::
                    findIndexByPredicate<std::string>(
                            vector, valueToFind,
                            predicateOfValueInCharStringEntry);
        }

      public:
        Entry<char, std::string> *findByValue(std::string &valueToFind) {
            int index = findIndexByValue(valueToFind);
            if (index ==
                VectorExtension<Entry<char, std::string>>::INDEX_NOT_FOUND) {
                return nullptr;
            }

            return vector[index];
        }
    };

  private:
    /**
     * @brief Indicates that the `Node` owns this char as a *value* was
     *        combined from two other `Node`s, and is considered as an
     *        *internal* `Node`.
     */
    static constexpr char INTERNAL_CHAR = '+';

  private:
    /**
     * @brief this function reads a *text* file and counts the frequency of
     *        each character appeared in the file.
     *
     * Each different char is converted to an `Entry` where the *key*
     * is the *frequency* that the char has appeared in the file, and the
     * *value* is the *character* itself.
     * @return `std::vector` of entries.
     */
    static std::vector<Entry<int, char> *> *countCharacters(char *fileName) {
        std::ifstream file(fileName);
        if (!file) { fileIsNullMessage(fileName); }

        auto vector = new std::vector<Entry<int, char> *>();
        countCharactersPrivate(file, *vector);
        file.close();

        return vector;
    }

  private:
    static void fileIsNullMessage(const char *fileName) {
        std::cerr << "The file `";
        std::cerr << fileName << "` wasn't found or couldn't be opened.";
    }

  private:
    /**
     * @see countCharacters(char *)
     */
    static void
    countCharactersPrivate(std::ifstream &                  file,
                           std::vector<Entry<int, char> *> &vector) {
        char seenCharacter;
        int  foundIndex;
        while (true) {
            file >> seenCharacter;
            if (file.eof()) { return; }

            // Try to find `seenCharacter` in the `vector`.
            foundIndex =
                    VectorExtension<Entry<int, char>>::findIndexByPredicate<
                            char>(vector, seenCharacter,
                                  predicateOfValueInIntCharEntry);
            if (foundIndex !=
                VectorExtension<Entry<int, char>>::INDEX_NOT_FOUND) {

                /*
                 * In case `seenCharacter` has been found in the `vector`,
                 * increase count.
                 */
                vector[foundIndex]->setKey(vector[foundIndex]->getKey() + 1);
            } else {

                /*
                 * In case `seenCharacter` has not been found in the `vector`,
                 * create an entry for this character and push it to `vector`.
                 */
                auto *entry = new Entry<int, char>(1, seenCharacter);
                vector.push_back(entry);
            }
        }
    }

  private:
    static bool predicateOfValueInIntCharEntry(Entry<int, char> &entry,
                                               char &            value) {
        return entry.getValue() == value;
    }

  private:
    static bool
    predicateOfValueInCharStringEntry(Entry<char, std::string> &entry,
                                      std::string &             value) {
        return entry.getValue() == value;
    }

  private:
    static bool predicateOfKeyInCharStringEntry(Entry<char, std::string> &entry,
                                                char &                    key) {
        return entry.getKey() == key;
    }

  private:
    static void writeToBinWithHuffmanTable(std::ofstream &ofstream,
                                           std::ifstream &ifstream,
                                           Table *        huffmanTable) {
        Serializer  serializer;
        char        seenCharacter;
        std::string binaryStringOfAllFile;

        /*
         * Read all characters in the file, an write their huffman-code to
         * a large string.
         */
        while (true) {
            ifstream >> seenCharacter;
            if (ifstream.eof()) { break; }

            binaryStringOfAllFile +=
                    (huffmanTable->findByKey(seenCharacter))->getValue();
        }

        // Attach the end-of-file marker.
        binaryStringOfAllFile +=
                (huffmanTable->findByKey(Huffman::Table::END_OF_FILE))
                        ->getValue();

        serializer.writeBinaryStringToBinaryFile(ofstream,
                                                 binaryStringOfAllFile);
    }

  private:
    static void writeToBinFile(char * fileNameToZip,
                               char * fileNameToOutputAsZipped,
                               Table *huffmanTable) {
        std::ofstream ofstream(fileNameToOutputAsZipped,
                               std::ios::out | std::ios::binary);
        if (!ofstream) { fileIsNullMessage(fileNameToOutputAsZipped); }

        // Write huffman-table to file.
        huffmanTable->serialize(ofstream);

        // Write file content.
        std::ifstream ifstream(fileNameToZip);
        if (!ifstream) { fileIsNullMessage(fileNameToZip); }
        writeToBinWithHuffmanTable(ofstream, ifstream, huffmanTable);

        delete huffmanTable;
    }

  public:
    /**
     * @brief This function receives a *text* file that contains *any*
     *        characters and *zips* it to another file, with the name of @p
     *        fileNameToOutputAsZipped.
     *
     * Before saving the file content itself, this function saves a
     * `huffman-table`, which presents how to `unzip` the file, for
     * future use.
     *
     * @param fileNameToZip the file-name to zip.
     * @param fileNameToOutputAsZipped the file-name to output as zipped.
     */
    static void zipFile(char *fileNameToZip, char *fileNameToOutputAsZipped) {
        std::vector<Entry<int, char> *> *vector =
                Huffman::countCharacters(fileNameToZip);
        Huffman::Table *huffmanTable = Huffman::huffmanize(*vector);
        writeToBinFile(fileNameToZip, fileNameToOutputAsZipped, huffmanTable);

        delete vector;
    }

  private:
    static void readFromBinWithHuffmanTable(std::ofstream &ofstream,
                                            std::ifstream &ifstream,
                                            Table *        huffmanTable) {
        Serializer  serializer;
        char        seenCharacter;
        std::string binaryStringOfAllFile;

        // Read all huffman-codes in the file, to a large string.
        binaryStringOfAllFile = serializer.readBinaryBitsToBinaryString(
                ifstream, seenCharacter, binaryStringOfAllFile);

        // TODO: debug
        std::cout << binaryStringOfAllFile << std::endl;

        writeHuffmanCodeFromBinaryStringToTextFile(ofstream, huffmanTable,
                                                   binaryStringOfAllFile);
    }

    static void writeHuffmanCodeFromBinaryStringToTextFile(
            std::ofstream &ofstream, Table *huffmanTable,
            std::string &binaryStringOfAllFile) {
        std::string currentCheckedString;
        for (int startIndex = 0, sizeSinceStartIndex = 1;
             startIndex < binaryStringOfAllFile.length();) {
            currentCheckedString = binaryStringOfAllFile.substr(
                    startIndex, sizeSinceStartIndex);
            Entry<char, std::string> *entry =
                    huffmanTable->findByValue(currentCheckedString);
            if (entry != nullptr) {

                /*
                 * This string is mapped to a character. Extract that
                 * character from the hash-table, and write to the output-file.
                 */
                if (entry->getKey() == Table::END_OF_FILE) {

                    // We have found the end-of-file marker - thus, quit.
                    break;
                }

                ofstream << entry->getKey();
                startIndex += sizeSinceStartIndex;
                sizeSinceStartIndex = 0;
            }
            ++sizeSinceStartIndex;
        }
    }

  public:
    static void unzipFile(char *fileNameToUnzip,
                          char *fileNameToOutputAsUnzipped) {
        std::ifstream ifstream(fileNameToUnzip,
                               std::ios::in | std::ios::binary);
        if (!ifstream) { fileIsNullMessage(fileNameToUnzip); }

        // Read huffman-table from file.
        auto *huffmanTable = new Huffman::Table();
        huffmanTable->deserialize(ifstream);

        // TODO: debug
        std::cout << *huffmanTable;

        // Read file content.
        std::ofstream ofstream(fileNameToOutputAsUnzipped);
        if (!ofstream) { fileIsNullMessage(fileNameToOutputAsUnzipped); }
        readFromBinWithHuffmanTable(ofstream, ifstream, huffmanTable);

        delete huffmanTable;
    }

  private:
    /**
     * @brief extracts a huffman-table from a given huffman-tree.
     *
     * @param huffmanTable this is a `new` `Huffman::Table`, that this
     *                     function will use as an output-parameter, for
     *                     the result huffman-tree.
     * @param node the root of the huffman-tree.
     * @param str the starting string for the root of the huffman-tree.
     */
    static void createHuffmanTableFromHuffmanTree(Huffman::Table * huffmanTable,
                                                  Node<int, char> *node,
                                                  std::string      str) {
        if (node == nullptr) { return; }
        if (node->getEntry() == nullptr) { return; }

        if (node->getEntry()->getValue() != Huffman::INTERNAL_CHAR) {
            huffmanTable->getVector().push_back(new Entry<char, std::string>(
                    node->getEntry()->getValue(), str));
        }

        createHuffmanTableFromHuffmanTree(huffmanTable, node->getLeftNode(),
                                          str + "0");
        createHuffmanTableFromHuffmanTree(huffmanTable, node->getRightNode(),
                                          str + "1");
    }

  private:
    /**
     * @brief This function returns the shortest way to represent each letter
     *        in via a given list of {'key': 'frequency-of-character', 'value':
     *        'character'}.
     *
     * This function creates a local huffman-tree and saves the
     * `huffman-code` for each character in the returned `Huffman::Table`.
     *
     * @param vector a list of {'key': 'frequency-of-character', 'value':
     *               'character'}.
     * @return a `Huffman::Table` from a list of characters and their
     *         frequencies.
     * @see Huffman::Table
     */
    static Huffman::Table *huffmanize(std::vector<Entry<int, char> *> &vector) {

        // Insert a end-of-file char, which will be used when zipping to binary.
        vector.push_back(new Entry<int, char>(1, Huffman::Table::END_OF_FILE));

        /*
         * Create a minimum-heap, and insert all characters of `vector`, so
         * we could create a huffman-tree from.
         */
        auto minHeapWrapper = new MinHeapWrapper<int, char>(vector);
        createHuffmanTree(*minHeapWrapper);

        // Create a huffman-table using the huffman-tree built.
        auto *huffmanTable = new Huffman::Table();

        try {
            createHuffmanTableFromHuffmanTree(
                    huffmanTable, minHeapWrapper->getMinHeap().top(), "");
        } catch (std::exception &e) { std::cout << e.what() << std::endl; }

        delete minHeapWrapper;
        return huffmanTable;
    }

  private:
    static void createHuffmanTree(MinHeapWrapper<int, char> &minHeapWrapper) {
        Node<int, char> *leftNode  = nullptr;
        Node<int, char> *rightNode = nullptr;
        Node<int, char> *topNode   = nullptr;
        while (minHeapWrapper.getMinHeap().size() != 1) {

            // Extract the two minimum frequent characters from minimum-heap.
            leftNode = minHeapWrapper.getMinHeap().top();
            minHeapWrapper.getMinHeap().pop();

            rightNode = minHeapWrapper.getMinHeap().top();
            minHeapWrapper.getMinHeap().pop();

            /*
             * Create a new internal node with frequency equal to the sum of the
             * two nodes frequencies. `Huffman::INTERNAL_CHAR` is a special value for
             * internal nodes.
             */
            topNode = new Node<int, char>(new Entry<int, char>(
                    leftNode->getEntry()->getKey() +
                            rightNode->getEntry()->getKey(),
                    INTERNAL_CHAR));

            /*
             *  Make the two extracted nodes as the `left`  and `right`
             *  children of this new node.
             */
            topNode->setLeftNode(leftNode);
            topNode->setRightNode(rightNode);

            // Push this node to the minimum-heap.
            minHeapWrapper.getMinHeap().push(topNode);
        }
    }
};

#endif // HUFFMAN_H
