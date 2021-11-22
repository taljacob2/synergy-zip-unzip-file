
#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <fstream>

/**
 * Interface.
 */
class Serializable {
    virtual void serialize(std::ofstream &file)   = 0;
    virtual void deserialize(std::ifstream &file) = 0;
};

#endif // SERIALIZABLE_H
