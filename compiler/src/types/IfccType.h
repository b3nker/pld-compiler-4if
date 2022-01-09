#pragma once

#include <cstddef>
#include <string>

/**
 * Abstract Type class
 */
class IfccType {
  public:
    IfccType(){};
    virtual ~IfccType(){};

    /**
     * @return A copy of the current object.
     */
    virtual IfccType *clone() const = 0;

    /**
     * For debug purposes.
     * @return The type's name in C language ("int", "char", ...).
     */
    virtual std::string getTypeName() const = 0;

    /**
     * @return The size (in bytes) needed to store one value of this type.
     */
    virtual size_t getSize() const = 0;
};
