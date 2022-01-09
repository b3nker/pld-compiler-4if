#pragma once

#include "IfccType.h"

/**
 * Represents a 32-bit integer type
 */
class IfccInt32_t : public IfccType {
  public:
    /**
     * For debug purposes.
     * @return The type's name in C language ("int").
     */
    std::string getTypeName() const { return "int"; };

    /**
     * @return The size (in bytes) needed to store one value of this type (4).
     */
    size_t getSize() const { return 4; };

    /**
     * Default constructor.
     */
    IfccInt32_t(){};
    /**
     * Default destructor.
     */
    ~IfccInt32_t(){};

    /**
     * @return A copy of the current object.
     */
    IfccInt32_t *clone() const { return new IfccInt32_t(*this); }
};
