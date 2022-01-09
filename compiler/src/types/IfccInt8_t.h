#pragma once

#include "IfccType.h"

/**
 * Represents an 8-bits integer type (character)
 */
class IfccInt8_t : public IfccType {
  public:
    /**
     * For debug purposes.
     * @return The type's name in C language ("char").
     */
    std::string getTypeName() const { return "char"; };

    /**
     * @return The size (in bytes) needed to store one value of this type (1).
     */
    size_t getSize() const { return 1; };

    /**
    * Default destructor.
    */
    ~IfccInt8_t(){};
    /**
     * Default constructor.
     */
    IfccInt8_t(){};

    /**
     * @return A copy of the current object.
     */
    IfccInt8_t *clone() const { return new IfccInt8_t(*this); }
};
