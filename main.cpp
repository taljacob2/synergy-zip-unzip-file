#include "Huffman.h"

/**
 * @mainpage Zip / Unzip file.
 *
 * @brief This program is able to zip / unzip a *text* file which contains
 *        *any* characters, except the one defined in `Huffman::Table::END_OF_FILE`.
 *
 * This program uses the *Huffman* algorithm to create a map, that translates
 * every character in the file, to a unique sequence of bits.
 *
 * @author Tal Jacob, email: taljacob2@gmail.com, phone: 054-6145961.
 */
int main() {
    Huffman::zipFile((char *) "test.txt", (char *) "zipped.bin");
    Huffman::unzipFile((char *) "zipped.bin", (char *) "unzipped.txt");

    return 0;
}
