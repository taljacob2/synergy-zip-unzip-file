#include "Huffman.h"

/**
 * @mainpage Zip / Unzip text file.
 *
 * @brief This program is able to zip / unzip a *text* file which contains
 *        *any* characters, except the one defined in `Huffman::Table::END_OF_FILE`.
 *
 * This program uses the *Huffman* algorithm to create a *map*, that translates
 * every character in the file, to a unique sequence of bits.
 *
 * The *map*, better called as a *huffman-table*, is serialized to the outputted
 * zip file - making it possible unzip the file at all times - making it
 * stable, and not affected by any other circumstances what so ever.
 *
 * @author Tal Jacob, email: taljacob2@gmail.com, phone: 054-6145961.
 */
int main() {
    Huffman::zipFile((char *) "text.txt", (char *) "zipped-text.bin");
    Huffman::unzipFile((char *) "zipped-text.bin",
                       (char *) "unzipped-text.txt");

    return 0;
}
