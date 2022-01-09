#pragma once

#include "IfccTypes.h"

/**
 * Represents an array type
 */
class IfccArray_t : public IfccType {
  private:
    /**
     * Initializes itemType, hasDefinedLength, and itemCount.
     * Called only by public constructors.
     * @param itemType
     * @param hasDefinedLength
     * @param itemCount
     */
    IfccArray_t(IfccType *itemType, bool hasDefinedLength, size_t itemCount)
        : itemType(itemType), hasDefinedLength(hasDefinedLength),
          itemCount(itemCount){};

  public:
    /**
     * The type of the array's members.
     */
    IfccType *itemType;    // e.g. int (IfccInt_t)
    /**
     * True if the array has a defined length (ie. number of members).
     */
    bool hasDefinedLength; // char[] vs char[42] → char[] ≈ char*
    /**
     * Size/length of the array.
     * Number of members (items) the array can contain.
     */
    size_t itemCount;      // bounds check

    /**
     * For debug purposes.
     * @return The type's name in C language ("int[]", "int[42]", "char[8]", ...).
     */
    std::string getTypeName() const {
        if (hasDefinedLength) {
            return itemType->getTypeName() + "[" + std::to_string(itemCount) +
                   "]";
        } else {
            return itemType->getTypeName() + "[]";
        }
    };

    /**
     * @return The size (in bytes) needed to store the array.
     */
    size_t getSize() const { return itemType->getSize() * itemCount; };

    /**
     * Public constructor with specified array length/size.
     * @param itemType Type of items the array can contain.
     * @param itemCount Number of item the array can contain.
     */
    IfccArray_t(IfccType *itemType, size_t itemCount)
        : IfccArray_t(itemType, true, itemCount){};

    /**
     * Public constructor without specified array length/size.
     * @param itemType
     */
    IfccArray_t(IfccType *itemType) : IfccArray_t(itemType, false, 0){};

    /**
     * Object destructor. Deletes itemType.
     */
    ~IfccArray_t() { delete itemType; };

    /**
     * @return A copy of the current object.
     */
    IfccArray_t *clone() const { return new IfccArray_t(*this); }
};
