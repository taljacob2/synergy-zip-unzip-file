#include "Huffman.h"

/**
 * @mainpage Zip / Unzip file.
 *
 * @brief This program is able to zip / unzip a *text* file that contains
 *        only the letters: a,b,c,d,e,f,g,h.
 *
 * @author Tal Jacob, email: taljacob2@gmail.com, phone: 054-6145961.
 */
int main() {
    Huffman::zipFile((char *) "test.txt", (char *) "zipped.bin");

    Huffman::unzipFile((char *) "zipped.bin", (char *) "unzipped.txt");

    return 0;
}
